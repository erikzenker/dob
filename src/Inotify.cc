#include <Inotify.h>


Inotify::Inotify() :
  mError(0),
  mEventMask(IN_ALL_EVENTS){
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
      dbg_print(LOG_ERR, "\nC Couldn´t initialize inotify, Errno: %d", mError);
      return false;
    }
    mIsInitialized = true;
  }
  return true;

}

bool Inotify::CleanUp(){
  assert(mIsInitialized);
  if(!close(mInotifyFd)){
    mError = errno;
    dbg_print(LOG_ERR, "\nC Can´t close inotify filedescriptor, Errno: %d", mError);
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
      dbg_print(LOG_DBG,"\nC Handle next file %s", nextFile.c_str());
      
      // Check the File/Folder
      if(lstat64(nextFile.c_str(), &my_stat) == -1){
	mError = errno;
	dbg_print(LOG_ERR, "\nC Error on fstat %s, %d", nextFile.c_str(), mError);
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
  dbg_print(LOG_DBG, "\nC Add watch to file %s", file.c_str());
  wd = inotify_add_watch(mInotifyFd, file.c_str(), mEventMask);
  if(wd == -1){
    mError = errno;
    dbg_print(LOG_ERR, "\nC Failed to watch %s, but keep on scanning, Errno: %d", file.c_str(), mError);
    return true;

  }
  return true;

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
      dbg_print(LOG_ERR, "\nC Couldn´t not opendir %s, Errno: %d", folder.c_str(), mError);

    }

  }
  return true;

}

inotify_event * Inotify::GetNextEvent(){
  inotify_event *event;
  int length = 0;
  int i = 0;
  char buffer[EVENT_BUF_LEN];
  length = read(mInotifyFd, buffer, EVENT_BUF_LEN);

  if(length == -1){
    mError = errno;
    dbg_print(LOG_ERR,"\nC Failed to read from inotify fd, Errno %d", mError);
    return NULL;
  }

  while(i < length){
    event = (struct inotify_event *) &buffer[i];
    mEventQueue.push(event);
    dbg_print(LOG_DBG,"\nC Event was triggered %s %s", event->name, EventMaskToString(event->mask).c_str());
    i += EVENT_SIZE + event->len;
  }

  if(mEventQueue.empty())
    return NULL;

  event = mEventQueue.front();
  mEventQueue.pop();
  return event;

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
