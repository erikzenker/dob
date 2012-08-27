#include <EventManager.h>

EventManager::EventManager(SyncManager * pSyncManager) : 
  mpSyncManager(pSyncManager){
  
  
}

void EventManager::PushBackEvent(inotify_event* pNewEvent, string sourceFolder){
  mEventList.push_back(pNewEvent);
  if(HandleEvent(pNewEvent, sourceFolder)){
    mEventList.pop_back();

  }
  else{
    cerr << "\nC Last event was not handled, need to be redone! (" << mEventList.size() << " event(s) left for handling)";
    
  }

}



