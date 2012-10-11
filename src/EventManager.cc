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
    cerr << "\nC Last event was handled";

  }
  else{
    cerr << "\nC Last event was not handled, need to be redone! (" << mEventList.size() << " event(s) left for handling)";
    if(!DispatchEvent(pNewEvent, sourceFolder))
      mEventList.pop_back();

  }
  SetScanIcon();

}

bool EventManager::DispatchEvent(inotify_event* const pEvent, const string sourceFolder){
  cerr << "\nC Dispatch event";
  string sync_folder(inotifytools_filename_from_wd(pEvent->wd));
  string folder(pEvent->name);
  string file_name("");

  file_name.append(sync_folder).append(folder);

  ifstream dispatch_file_stream;
  dispatch_file_stream.open(file_name.c_str());
  if(dispatch_file_stream.good()){
    return !HandleEvent(pEvent, sourceFolder);
    
  }
  cerr << "\nC Tried to dispatch event, but event was tropped because file is not there anymore";
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



