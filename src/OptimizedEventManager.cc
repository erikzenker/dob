#include <OptimizedEventManager.h>

OptimizedEventManager::OptimizedEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool OptimizedEventManager::HandleEvent(inotify_event* pEvent, string sourceFolder){
  // cerr <<"\nC Event " << inotifytools_event_to_str(pEvent->mask) <<" was triggered";
  string syncFolder(inotifytools_filename_from_wd(pEvent->wd));
  string folder(pEvent->name);

  switch(pEvent->mask){
  case IN_CREATE:
    return(mpSyncManager->SyncFolder(sourceFolder, syncFolder, folder));
    break;
  case IN_DELETE:
    return(mpSyncManager->RemoveFolder(sourceFolder, syncFolder, folder));
    break;
  case IN_MODIFY:
    return(mpSyncManager->SyncFolder(sourceFolder, syncFolder, folder));
    break;
  case IN_CREATE | IN_ISDIR:
    return(mpSyncManager->SyncFolder(sourceFolder, syncFolder, folder));
    break;
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->RemoveFolder(sourceFolder, syncFolder, folder));
    break;
  default:
    dbg_print(LOG_DBG, "\nC OptimizedEventManager::HandleEvent: No handler for this event implementet: ");
    break;

  }
  return false;
}


