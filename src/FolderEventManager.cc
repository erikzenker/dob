#include <FolderEventManager.h>

FolderEventManager::FolderEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool FolderEventManager::handleEvent(FileSystemEvent<int>* pEvent, string sourceFolder){
  std::string syncFolder = pEvent->getWatchFolder();
  std::string folder = pEvent->getFilename();

  switch(pEvent->getMask()){
  case IN_MOVED_TO:
  case IN_MODIFY:
  case IN_CREATE:
    return(mpSyncManager->syncFile(sourceFolder, syncFolder));
    break;
  case IN_CREATE | IN_ISDIR:
    return(mpSyncManager->syncFolder(sourceFolder, syncFolder, folder));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->removeFolder(sourceFolder, syncFolder, folder));
    break;
  default:
    dbg_printc(LOG_ERR, "FolderEventManager", "HandleEvent", "No handler for this event implementet: %s",pEvent->getMaskString().c_str());
    break;

  }
  return false;
}


