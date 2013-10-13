#include <FolderEventManager.h>

FolderEventManager::FolderEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool FolderEventManager::handleEvent(FileSystemEvent* pEvent, string sourceFolder){
  boost::filesystem::path eventPath = pEvent->getPath();

  switch(pEvent->getMask()){
  case IN_MODIFY:
    return(mpSyncManager->syncFile(sourceFolder, eventPath, FS_MODIFY));
    break;
  case IN_MOVED_TO:
  case IN_CREATE:
    return(mpSyncManager->syncFile(sourceFolder, eventPath, FS_CREATE));
    break;
  case IN_CREATE | IN_ISDIR:
    return(mpSyncManager->syncFolder(sourceFolder, eventPath));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->removeFolder(sourceFolder, eventPath));
    break;
  default:
    dbg_printc(LOG_ERR, "FolderEventManager", "HandleEvent", "No handler for this event implementet: %s",pEvent->getMaskString().c_str());
    return true;
    break;

  }
  return false;
}


