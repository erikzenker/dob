#include <EventManager.h>

EventManager::EventManager(SyncManager * const pSyncManager) : 
  mpSyncManager(pSyncManager){
  
  
}

SyncManager* EventManager::GetSyncManager() const{
  return mpSyncManager;

}

void EventManager::PushBackEvent(inotify_event* const pNewEvent, const string sourceFolder){
  SetSyncIcon();
  mEventList.push_back(pNewEvent);
  if(HandleEvent(pNewEvent, sourceFolder)){
    mEventList.pop_back();
    cerr << "\nC Last event was handled";

  }
  else{
    cerr << "\nC Last event was not handled, need to be redone! (" << mEventList.size() << " event(s) left for handling)";
    
  }

  //if(mEventList.size() == 0){
    SetScanIcon();
    //}

}

void EventManager::SetPauseIcon() const{
  mEventManagerSignal.emit(false, 0);
}

void EventManager::SetSyncIcon() const{
  mEventManagerSignal.emit(false, 1);
}

void EventManager::SetScanIcon() const{
  mEventManagerSignal.emit(false, 2);
}

EventManagerSignal EventManager::SignalEvent(){
  return mEventManagerSignal;

}



