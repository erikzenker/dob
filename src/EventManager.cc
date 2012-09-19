#include <EventManager.h>

EventManager::EventManager(SyncManager * const pSyncManager) : 
  mpSyncManager(pSyncManager){
  
  
}

SyncManager* EventManager::GetSyncManager() const{
  return mpSyncManager;

}

void EventManager::PushBackEvent(inotify_event* const pNewEvent, const string sourceFolder){
  //mrStatusIcon->set(Gtk::Stock::REFRESH);
  mEventList.push_back(pNewEvent);
  if(HandleEvent(pNewEvent, sourceFolder)){
    mEventList.pop_back();

  }
  else{
    cerr << "\nC Last event was not handled, need to be redone! (" << mEventList.size() << " event(s) left for handling)";
    
  }
  if(mEventList.size() == 0){
    //mrStatusIcon->set(Gtk::Stock::APPLY);
  }

}



