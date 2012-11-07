#include <Inotify.h>


Inotify::Inotify() :
  mError(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE){
  Initialize();
}

Inotify::~Inotify(){
  CleanUp();
}

bool Inotify::Initialize(){
  // Try to initialise inotify
  if(!mIsInitialized){
    mInotifyFd = inotify_init();
    if(mInotifyFd == -1){
      mError = errno;
      dbg_printc(LOG_ERR, "Inotify", "Initialize", "Couldn´t initialize inotify, Errno: %d", mError);
      return false;
    }
    mIsInitialized = true;
  }
  dbg_printc(LOG_DBG, "Inotify", "Initialize", "Inotify is initialized now, fd: %d", mInotifyFd);
  return true;

}

bool Inotify::CleanUp(){
  assert(mIsInitialized);
  if(!close(mInotifyFd)){
    mError = errno;
    dbg_printc(LOG_ERR,"Inotify","CleanUp", "Can´t close inotify fd(%d), Errno: %d", mInotifyFd, mError);
    return false;
  }
  return true;
}

bool Inotify::WatchFolderRecursively(std::string watchFolder){
  assert(mIsInitialized);
  // Important : Initialize should be called in advance
  DIR * directory;
  std::string nextFile = "";
  struct dirent * ent;
  struct stat64 my_stat;

  directory = opendir(watchFolder.c_str());
  if(!IsDir(watchFolder))
    return WatchFile(watchFolder);


  if(watchFolder[watchFolder.size()-1] != '/'){
    watchFolder.append("/");
  }

  ent = readdir(directory);
  // Watch each directory within this directory
  while(ent){
    if((0 != strcmp(ent->d_name, ".")) && (0 != strcmp(ent->d_name, ".."))){
      nextFile = watchFolder;
      nextFile.append(ent->d_name);
      
      // Check the File/Folder
      if(lstat64(nextFile.c_str(), &my_stat) == -1){
	mError = errno;
	dbg_printc(LOG_ERR, "Inotify", "WatchFolderRecursively", "\nC Error on fstat %s, %d", nextFile.c_str(), mError);
	if (mError != EACCES){
	  mError = errno;
	  closedir(directory);
	  return false;

	}

      }
      // Watch a folder recursively
      else if(S_ISDIR(my_stat.st_mode ) || S_ISLNK( my_stat.st_mode )) {
	nextFile.append("/");
	bool status = WatchFolderRecursively(nextFile);
	
	if (!status){
	  closedir(directory);
	  return false;

	}

      }
      // Watch just a File
      else if(S_ISREG(my_stat.st_mode)){
	WatchFile(nextFile);

      }

    }
    ent = readdir(directory);
    mError = 0;
  }
  closedir(directory);
  // Finally add watch to parentfolder
  return WatchFile(watchFolder);
}


bool Inotify::WatchFile(std::string file){
  assert(mIsInitialized);
  mError = 0;
  int wd;
  dbg_printc(LOG_DBG, "Inotify","WatchFile","Add watch of file %s", file.c_str());
  wd = inotify_add_watch(mInotifyFd, file.c_str(), mEventMask);
  if(wd == -1){
    mError = errno;
    dbg_printc(LOG_ERR, "Inotify", "WatchFile", "Failed to watch %s, but keep on scanning, Errno: %d", file.c_str(), mError);
    return true;

  }
  mFolderMap[wd] =  file;
  return true;

}

std::string Inotify::WdToFilename(int wd){
  assert(mIsInitialized);
  return mFolderMap[wd];

}

bool Inotify::IsDir(std::string folder){
  DIR* directory;
  directory = opendir(folder.c_str());
  if(!directory) {
    mError = errno;
    if(mError == ENOTDIR){
      return false;

    }
    else {
      dbg_printc(LOG_ERR, "Inotify","IsDir", "Couldn´t not opendir %s, Errno: %d", folder.c_str(), mError);

    }

  }
  return true;

}

FileSystemEvent<int>*  Inotify::GetNextEvent(){
  assert(mIsInitialized);
  inotify_event *event;
  FileSystemEvent<int> *fileSystemEvent;
  int length = 0;
  int i = 0;
  char buffer[EVENT_BUF_LEN];

  if(mEventQueue.empty()){
    length = read(mInotifyFd, buffer, EVENT_BUF_LEN);
    while(length <= 0){
      if(length == -1){
	mError = errno;
	if(mError != EINTR){
	  dbg_printc(LOG_ERR,"Inotify", "GetNextEvent", "Failed to read from inotify fd(%d), Errno %d",mInotifyFd, mError);
	  return NULL;

	}
	dbg_printc(LOG_ERR,"Inotify", "GetNextEvent", "Inotify read EINTR from fd(%d)",mInotifyFd );

      }
      sleep(1);
    }

    while(i < length){
      event = (struct inotify_event *) &buffer[i];
      fileSystemEvent = new FileSystemEvent<int>(event->wd, event->mask, event->name, WdToFilename(event->wd)); 
      mEventQueue.push(fileSystemEvent);
      dbg_printc(LOG_DBG, "Inotify", "GetNextEvent",
		"Event from fd(%d) was triggered %s %d %s", 
		mInotifyFd,
		fileSystemEvent->GetFilename().c_str(), 
		fileSystemEvent->GetMask(),
		fileSystemEvent->GetMaskString().c_str());
      i += EVENT_SIZE + event->len;
    }

  }
  if(mEventQueue.empty())
    return NULL;

  fileSystemEvent = mEventQueue.front();
  mEventQueue.pop();
  return fileSystemEvent;

}

std::string Inotify::EventMaskToString(uint32_t events){
    std::string eventString = "";

  if(IN_ACCESS & events)
    eventString.append("IN_ACCESS ");
  if(IN_ATTRIB & events)
    eventString.append("IN_ATTRIB ");
  if(IN_CLOSE_WRITE & events)
    eventString.append("IN_CLOSE_WRITE ");
  if(IN_CLOSE_NOWRITE & events)
    eventString.append("IN_CLOSE_NOWRITE ");
  if(IN_CREATE & events)
    eventString.append("IN_CREATE");
  if(IN_DELETE & events)
    eventString.append("IN_DELETE");
  if(IN_DELETE_SELF & events)
    eventString.append("IN_DELETE_SELF");
  if(IN_MODIFY & events)
    eventString.append("IN_MODIFY");
  if(IN_MOVE_SELF & events)
    eventString.append("IN_MOVE_SELF");
  if(IN_MOVED_FROM & events)
    eventString.append("IN_MOVED_FROM");
  if(IN_MOVED_TO & events)
    eventString.append("IN_MOVED_TO");
  if(IN_OPEN & events)
    eventString.append("IN_OPEN");

  return eventString;
}

int Inotify::GetLastError(){
  return mError;

}