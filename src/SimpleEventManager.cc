#include <SimpleEventManager.h>

SimpleEventManager::SimpleEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool SimpleEventManager::handleEvent(FileSystemEvent<int>* pEvent, string sourceFolder){
  //cerr <<"\nC Event " << inotifytools_event_to_str(pEvent->mask) <<" was triggered";
  string syncFolder = pEvent->getWatchFolder();
  syncFolder.append(pEvent->getFilename());
  switch(pEvent->getMask()){
  case IN_CREATE:
    mpSyncManager->syncSourceFolder(sourceFolder);
    break;
  case IN_DELETE:
    mpSyncManager->syncSourceFolder(sourceFolder);
    break;
  case IN_MODIFY:
    mpSyncManager->syncSourceFolder(sourceFolder);
    break;
  case IN_CREATE | IN_ISDIR:
    mpSyncManager->syncSourceFolder(sourceFolder);
    break;
  case IN_DELETE | IN_ISDIR:
    mpSyncManager->syncSourceFolder(sourceFolder);
    break;
  default:
    cerr << "\nC SimpleEventManager::HandleEvent: No handler for this event implementet ";
    break;

  }
}


