#include <Inotify.h>


Inotify::Inotify() :
  mError(0),
  mEventTimeout(0),
  mLastEventTime(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE){
  Initialize();
}

Inotify::Inotify(std::vector<std::string> ignoredFolders) :
  mError(0),
  mEventTimeout(0),
  mLastEventTime(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE),
  mIgnoredFolders(ignoredFolders){

  Initialize();
}

Inotify::Inotify(std::string ignoredFolder) :
  mError(0),
  mEventTimeout(0),
  mLastEventTime(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE){

  Initialize();
  mIgnoredFolders.push_back(ignoredFolder);
}

  Inotify::Inotify(std::string ignoredFolder, int eventTimeout) :
  mError(0),
  mEventTimeout(eventTimeout),
  mLastEventTime(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE){

  Initialize();
  mIgnoredFolders.push_back(ignoredFolder);
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
      
      // Check the File/Folder for acces
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
	if(!IsIgnored(nextFile)){
	  bool status = WatchFolderRecursively(nextFile);
	  if (!status){
	    closedir(directory);
	    return false;

	  }

	}

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
    dbg_printc(LOG_WARN, "Inotify", "WatchFile", "Failed to watch %s, but keep on scanning, Errno: %d", file.c_str(), mError);
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

    }
    else {
      dbg_printc(LOG_WARN, "Inotify","IsDir", "Couldn´t not opendir %s, Errno: %d", folder.c_str(), mError);


    }
    closedir(directory);
    return false;

  }
  closedir(directory);
  return true;

}

FileSystemEvent<int>*  Inotify::GetNextEvent(){
  assert(mIsInitialized);
  inotify_event *event;
  FileSystemEvent<int> *fileSystemEvent;
  int length = 0;
  int i = 0;
  char buffer[EVENT_BUF_LEN];
  time_t currentEventTime = time(NULL);

  // Read Events from fd
  if(mEventQueue.empty() || OnTimeout(currentEventTime)){
    // Clear EventQueue when still in Timeout
    if(OnTimeout(currentEventTime))
      ClearEventQueue();

    while(length <= 0 || OnTimeout(currentEventTime)){
      //dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "currentEventTime: %d", currentEventTime );
      //dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "lastEventTime + eventTimeout: %d", mLastEventTime + mEventTimeout );
      //dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "Read for next event", currentEventTime );
      length = read(mInotifyFd, buffer, EVENT_BUF_LEN);
      currentEventTime = time(NULL);
      if(length == -1){
	mError = errno;
	if(mError != EINTR){
	  dbg_printc(LOG_ERR,"Inotify", "GetNextEvent", "Failed to read from inotify fd(%d), Errno %d", mInotifyFd, mError);
	  return NULL;

	}

	dbg_printc(LOG_WARN, "Inotify", "GetNextEvent", "Can´t read from Inotify fd(%d), Errno: EINTR, try to read again", mInotifyFd );

      }

    }

  }
  // Read Events from buffer
  //dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "currentEventTime: %d", currentEventTime );
  //dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "lastEventTime + eventTimeout: %d", mLastEventTime + mEventTimeout );

  mLastEventTime = currentEventTime;
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

bool Inotify::IsIgnored(std::string file){
  if(!mIgnoredFolders[0].compare(""))
    return false;
  for(int i = 0; i < mIgnoredFolders.size(); ++i){
    size_t pos = file.find(mIgnoredFolders[i]);
    if(pos!= string::npos){
      //dbg_printc(LOG_DBG, "Inotify", "IsIgnored","mI: %s", file.c_str());
      dbg_printc(LOG_DBG, "Inotify", "IsIgnored","File will be ignored: %s", file.c_str());
      return true;
    }
  }
  return false;
}

void Inotify::ClearEventQueue(){
  int eventCount;
  for(eventCount = 0; eventCount < mEventQueue.size(); eventCount++){
    mEventQueue.pop();
  }

}

bool Inotify::OnTimeout(time_t eventTime){
  return (mLastEventTime + mEventTimeout) > eventTime;

}
