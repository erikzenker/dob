#include <SimpleEventManager.h>

SimpleEventManager::SimpleEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool SimpleEventManager::HandleEvent(FileSystemEvent<int>* pEvent, string sourceFolder){
  //cerr <<"\nC Event " << inotifytools_event_to_str(pEvent->mask) <<" was triggered";
  string syncFolder = pEvent->GetWatchFolder();
  syncFolder.append(pEvent->GetFilename());
  switch(pEvent->GetMask()){
  case IN_CREATE:
    mpSyncManager->SyncSourceFolder(sourceFolder);
    break;
  case IN_DELETE:
    mpSyncManager->SyncSourceFolder(sourceFolder);
    break;
  case IN_MODIFY:
    mpSyncManager->SyncSourceFolder(sourceFolder);
    break;
  case IN_CREATE | IN_ISDIR:
    mpSyncManager->SyncSourceFolder(sourceFolder);
    break;
  case IN_DELETE | IN_ISDIR:
    mpSyncManager->SyncSourceFolder(sourceFolder);
    break;
  default:
    cerr << "\nC SimpleEventManager::HandleEvent: No handler for this event implementet ";
    break;

  }
}


