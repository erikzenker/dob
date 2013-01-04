#include <OptimizedEventManager.h>

OptimizedEventManager::OptimizedEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool OptimizedEventManager::HandleEvent(FileSystemEvent<int>* pEvent, string sourceFolder){
  string syncFolder = pEvent->GetWatchFolder();
  string folder = pEvent->GetFilename();

  switch(pEvent->GetMask()){
  case IN_MOVED_TO:
  case IN_MODIFY:
  case IN_CREATE:
    return(mpSyncManager->SyncFile(sourceFolder, syncFolder));
    break;
  case IN_CREATE | IN_ISDIR:
    return(mpSyncManager->SyncFolder(sourceFolder, syncFolder, folder));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->RemoveFolder(sourceFolder, syncFolder, folder));
    break;
  default:
    dbg_printc(LOG_ERR, "OptimizedEventManager", "HandleEvent", "No handler for this event implementet: %s",pEvent->GetMaskString().c_str());
    break;

  }
  return false;
}


