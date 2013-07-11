#include <Inotify.h>

Inotify::Inotify(std::vector<std::string> ignoredFolders, int eventTimeout) :
  mError(0),
  mEventTimeout(eventTimeout),
  mLastEventTime(0),
  mEventMask(IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE),
  mIgnoredFolders(ignoredFolders),
  mIsInitialized(false),
  mInotifyFd(0){
  
  initialize();

}

Inotify::~Inotify(){
  assert(mIsInitialized);
  if(!close(mInotifyFd)){
    mError = errno;
    dbg_printc(LOG_ERR,"Inotify","CleanUp", "Can´t close inotify fd(%d), Errno: %d", mInotifyFd, mError);
  }
}

bool Inotify::initialize(){
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

bool Inotify::watchFolderRecursively(std::string watchPath){
  boost::filesystem::path path(watchPath);
  if(boost::filesystem::is_directory(watchPath)){
      boost::filesystem::recursive_directory_iterator it(path);
      boost::filesystem::recursive_directory_iterator end;
  
      while(it != end){

	if(boost::filesystem::is_regular_file(*it)){
	  watchFile(*it);
	}
	if(boost::filesystem::is_directory(*it)){
	  watchFile(*it);
	}

	++it;

      }

    }
  return watchFile(path);
}

bool Inotify::watchFile(boost::filesystem::path file){
  assert(mIsInitialized);
  mError = 0;
  int wd;
  if(!isIgnored(file.string())){
    dbg_printc(LOG_DBG, "Inotify","WatchFile","Add watch of file: %s", file.string().c_str());
    wd = inotify_add_watch(mInotifyFd, file.string().c_str(), mEventMask);
  }
  else{
    return true;
  }

  if(wd == -1){
    mError = errno;
    if(mError == 28){
      dbg_printc(LOG_WARN, "Inotify", "WatchFile", "Failed to watch %s, please increase number of watches in /proc/sys/fs/inotify/max_user_watches , Errno: %d", file.string().c_str(), mError);
    }
    else{
      dbg_printc(LOG_WARN, "Inotify", "WatchFile", "Failed to watch %s, but keep on scanning, Errno: %d", file.string().c_str(), mError);
    }
    return true;

  }
  mFolderMap[wd] =  file.string();
  return true;

}

bool Inotify::removeWatch(int wd){
  int error = inotify_rm_watch(mInotifyFd, wd);
  if(errno <= 0){
    dbg_printc(LOG_WARN, "Inotify","RemoveWatch","Failed to remove watch of wd %d, errno: %d", wd, error);
    return false;
  }
  dbg_printc(LOG_DBG, "Inotify","WatchFile","Removed watch with wd: %d, file: %s", wd, wdToFilename(wd).c_str());
  mFolderMap.erase(wd);
  return true;
}

std::string Inotify::wdToFilename(int wd){
  assert(mIsInitialized);
  return mFolderMap[wd];

}

FileSystemEvent<int>*  Inotify::getNextEvent(){
  assert(mIsInitialized);
  inotify_event *event;
  FileSystemEvent<int> *fileSystemEvent;
  int length = 0;
  int i = 0;
  char buffer[EVENT_BUF_LEN];
  time_t currentEventTime = time(NULL);
  std::vector<FileSystemEvent<int>* > newEvents;


  // Read Events from fd into buffer
  if(mEventQueue.empty()){
    while(length <= 0 ){
      dbg_printc(LOG_DBG, "Inotify", "GetNextEvent", "Read from inotify fd(%d)", mInotifyFd );
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

  // Read events from buffer into queue
  currentEventTime = time(NULL);
  while(i < length){
    event = (struct inotify_event *) &buffer[i];
    fileSystemEvent = new FileSystemEvent<int>(event->wd, event->mask, event->name, wdToFilename(event->wd)); 
    if(checkEvent(fileSystemEvent) && fileSystemEvent->getMask() != IN_IGNORED){
      newEvents.push_back(fileSystemEvent);
      dbg_printc(LOG_DBG, "Inotify", "GetNextEvent",
		 "Event from fd(%d) was triggered %s %d %s queue.length: %d", 
		 mInotifyFd,
		 fileSystemEvent->getFilename().c_str(), 
		 fileSystemEvent->getMask(),
		 fileSystemEvent->getMaskString().c_str(),
		 newEvents.size());
    }
    i += EVENT_SIZE + event->len;

  }

  // Filter events for timeout
  std::vector<FileSystemEvent<int>* >::iterator eventIter;
  for(eventIter = newEvents.begin(); eventIter < newEvents.end(); ++eventIter){
    if(onTimeout(currentEventTime)){
      newEvents.erase(eventIter);
      dbg_printc(LOG_DBG, "Inotify", "GetNextEvent"," Erase Event");
    
    }
    else{
      mLastEventTime = currentEventTime;
      mEventQueue.push(*eventIter);
    }

  }

  // Choose next event
  if(mEventQueue.empty())
    fileSystemEvent = NULL;

  else{
    fileSystemEvent = mEventQueue.front();
    mEventQueue.pop();
  }

  return fileSystemEvent;

}

int Inotify::getLastError(){
  return mError;

}

bool Inotify::isIgnored(std::string file){
  //std::regex ignoreRegex;

  if(mIgnoredFolders.empty()){
    return false;
  }

  // for(int i = 0; i < mIgnoredFolders.size(); ++i){
  //   ignoreRegex.assign(mIgnoredFolders[i], std::regex_constants::ECMAScript);
  //   if(std::regex_match(file, ignoreRegex)){
  //     dbg_printc(LOG_DBG, "Inotify", "IsIgnored","File will be ignored: %s", file.c_str());
  //     return true;
  //   }
      
  // }

  for(unsigned i = 0; i < mIgnoredFolders.size(); ++i){
    size_t pos = file.find(mIgnoredFolders[i]);
    if(pos!= std::string::npos){
      dbg_printc(LOG_DBG, "Inotify", "IsIgnored","File will be ignored: %s", file.c_str());
      return true;
    }
  }
  return false;
}

void Inotify::clearEventQueue(){
  for(unsigned eventCount = 0; eventCount < mEventQueue.size(); eventCount++){
    mEventQueue.pop();
  }

}

bool Inotify::onTimeout(time_t eventTime){
  return (mLastEventTime + mEventTimeout) > eventTime;

}

bool Inotify::checkEvent(FileSystemEvent<int>* event){
  // Events seems to have no syncfolder,
  // this can happen if not the full event
  // was read from inotify fd
  if(!event->getWatchFolder().compare("")){
    return false;
  }

  return true;

}
