#include "InotifyFileSystemScanner.h"

InotifyFileSystemScanner::InotifyFileSystemScanner(const std::string scanFolder, std::vector<std::string> ignoredFolders, const int eventTimeout, EventManager* const pEventManager) 
  : FileSystemScanner(scanFolder, pEventManager ){

  mInotify = new Inotify(ignoredFolders, eventTimeout);

}

InotifyFileSystemScanner::~InotifyFileSystemScanner(){
  
}

int InotifyFileSystemScanner::startToScan(){
  return start(NULL);
}

int InotifyFileSystemScanner::stopToScan(){
  return stop();
}

void InotifyFileSystemScanner::setup(){
  
}

/**
 * @brief Threaded scanning of scanFolder
 *
 * With help of inotify the scanFolder will be watched
 * recursively for events. If events occur they will
 * be pushed to an eventManager. New folders/files will 
 * also be watched again or removed from watches.
 *
 * @todo when deleting a symlink from a watched folder, then
 *       the folder the symlink points to should be removed
 *       from watched folders.
 *       IDEA: save symlinks in kind of database so you
 *             know the pointed folder
 *	 Handle more then one event at a time
 * 
 **/
void InotifyFileSystemScanner::execute(void* arg){
  // Create file watches
  if(!mInotify->watchFolderRecursively(mScanFolder)){
    dbg_printc(LOG_ERR,"InotifyFileSystemScanner", "Execute", "Failed to watch recursively errno: %d", mInotify->getLastError());

  }

  // Update changed files to server

  dbg_printc(LOG_DBG,"InotifyFileSystemScanner", "Execute", "Start scanning folder: %s", mScanFolder.c_str());
  FileSystemEvent<int> * fileSystemEvent = mInotify->getNextEvent();

  while(fileSystemEvent){
    mpEventManager->pushBackEvent(fileSystemEvent, mScanFolder);

    // Add or delete watches for added/deleted folders or files
    switch(fileSystemEvent->getMask()){
    case IN_MOVED_FROM:
    case IN_DELETE:
    case IN_DELETE | IN_ISDIR:
      // @todo remove watches recursively
      // @bug removing watches also removes information
      //      about the location of a watch
      // @todo think about this situation
      //mInotify->watchFile(fileSystemEvent->getFolderPath());
      //mInotify->removeWatch(fileSystemEvent->getId());
      break;

    case IN_MOVED_TO:
    case IN_CREATE:
    case IN_CREATE | IN_ISDIR:
      dbg_printc(LOG_DBG, "InotifyFileSystemScanner", "Execute", "Add new watch file: %s", fileSystemEvent->getFilename().c_str());
      mInotify->watchFolderRecursively(fileSystemEvent->getFullPath());
      break;

    case IN_MODIFY:
      // Do nothing
      break;
    default:
      dbg_printc(LOG_ERR, 
		 "InotifyFileSystemScanner", 
		 "Execute",
		 "Unexpected event was triggered %s %s",fileSystemEvent->getMaskString().c_str(), fileSystemEvent->getFilename().c_str());
    }

    // Handle next event and ignore NULL-Events
    fileSystemEvent = NULL;
    while(!fileSystemEvent){
      fileSystemEvent = mInotify->getNextEvent();
    }

  }
      
}


