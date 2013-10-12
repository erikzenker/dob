#include <FileEventManager.h>
#include <boost/filesystem.hpp>
#include <unistd.h>
#include <iostream>

FileEventManager::FileEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool FileEventManager::handleEvent(FileSystemEvent* pEvent, string sourceFolder){
  boost::filesystem::path eventPath = pEvent->getPath();

  switch(pEvent->getMask()){
  case IN_MOVED_TO:
  case IN_MODIFY:
  case IN_CREATE:
    return(mpSyncManager->syncFile(sourceFolder, eventPath.string()));
    break;
  case IN_CREATE | IN_ISDIR:
    // Delay gives os time to cp data into folder
    usleep(1000000); 
    return(mpSyncManager->syncFolder(sourceFolder, eventPath.string(), ""));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
    return(mpSyncManager->removeFolder(sourceFolder, eventPath.string(), ""));
    break;
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->removeFolder(sourceFolder, eventPath.string(), "/"));
    break;
  default:
    dbg_printc(LOG_ERR, "FileEventManager", "HandleEvent", "No handler for this event implementet: %s",pEvent->getMaskString().c_str());
    return true;
    break;

  }
  return false;
}


