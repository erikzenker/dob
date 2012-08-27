#include <SimpleEventManager.h>

SimpleEventManager::SimpleEventManager(SyncManager * pSyncManager)
  : EventManager(pSyncManager){
}

bool SimpleEventManager::HandleEvent(inotify_event* pEvent, string sourceFolder){
  cerr <<"\nC Event " << inotifytools_event_to_str(pEvent->mask) <<" was triggered";
  string syncFolder(inotifytools_filename_from_wd(pEvent->wd));
  syncFolder.append(pEvent->name);
  cerr << "\nC " << syncFolder;
  switch(pEvent->mask){
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
    cerr << "\nC No handler for this event implementet: " << inotifytools_event_to_str(pEvent->mask);
    break;

  }
}


