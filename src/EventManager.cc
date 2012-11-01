#include <EventManager.h>

EventManager::EventManager(SyncManager * const pSyncManager) : 
  mpSyncManager(pSyncManager){
    
}

SyncManager* EventManager::GetSyncManager() const{
  return mpSyncManager;

}

/**
 * @bug mrStatusIcon->set(Icon): leads sometimes to a hidden status icon 
 *      in the system tray with hidden popup menu.
 *      
 *
 **/
void EventManager::PushBackEvent(inotify_event* const pNewEvent, const string sourceFolder){
  SetSyncIcon();
  mEventList.push_back(pNewEvent);
  if(HandleEvent(pNewEvent, sourceFolder)){
    mEventList.pop_back();
    dbg_print(LOG_DBG, "\nC EventManager::PushBackEvent: Last event was handled");

  }
  else{
    dbg_print(LOG_DBG, "\nC EventManager::PushBackEvent: Last event was not handled, need to be redone! (%d event(s) left for handling)", mEventList.size());
    if(!DispatchEvent(pNewEvent, sourceFolder))
      mEventList.pop_back();

  }
  usleep(100);
  SetScanIcon();

}

bool EventManager::DispatchEvent(inotify_event* const pEvent, const string sourceFolder){
  dbg_print(LOG_DBG, "\nC EventManager::DispatchEvent: Dispatch event");
  string sync_folder(inotifytools_filename_from_wd(pEvent->wd));
  string folder(pEvent->name);
  string file_name("");

  file_name.append(sync_folder).append(folder);

  ifstream dispatch_file_stream;
  dispatch_file_stream.open(file_name.c_str());
  if(dispatch_file_stream.good()){
    return !HandleEvent(pEvent, sourceFolder);
    
  }
  dbg_print(LOG_DBG, "\nC EventManager::DispatchEvent: Tried to dispatch event, but event was tropped because file is not there anymore");
  return false;

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



