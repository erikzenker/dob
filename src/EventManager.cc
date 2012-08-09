#include <EventManager.h>

EventManager::EventManager(SyncManager * pSyncManager){
  mpSyncManager = pSyncManager;

}

void EventManager::PushBackEvent(inotify_event* pNewEvent, string sourceFolder){
  mEventList.push_back(pNewEvent);
  HandleEvent(pNewEvent, sourceFolder);

}



