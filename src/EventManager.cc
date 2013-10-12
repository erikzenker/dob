#include <EventManager.h>
#include <boost/filesystem.hpp>

EventManager::EventManager(SyncManager * const pSyncManager) : 
  mpSyncManager(pSyncManager){
    
}

EventManager::~EventManager(){
  free(mpSyncManager);
}

SyncManager* EventManager::getSyncManager() const{
  return mpSyncManager;

}

/**
 * @brief Events can be pushed back and will be queued.
 *
 * Events will be forwarded to handleEvent and if it
 * was impossible to handle the event, because maybe the
 * file is not there anymore, the event will be 
 * dispatched.
 *
 * @bug   SetSyncIcon will make StatusIcon invisible
 * @param pNewEvent    Event you want to process
 * @param sourceFolder The scanned folder
 *
 **/
void EventManager::pushBackEvent(FileSystemEvent* const pNewEvent, const std::string sourceFolder){
  mEventList.push_back(pNewEvent);
  if(handleEvent(pNewEvent, sourceFolder)){
    mEventList.pop_back();
    dbg_printc(LOG_DBG, 
	       "EventManager",
	       "PushBackEvent",
	       "Last event was handled %s %d %s",
	       pNewEvent->getPath().string().c_str(), 
	       pNewEvent->getMask(),
	       pNewEvent->getMaskString().c_str());


  }
  else{
    dbg_printc(LOG_DBG, 
	       "EventManager",
	       "PushBackEvent",
	       "Last event was not handled %s %d %s, need to be redone! (%d event(s) left for handling)", 
	       pNewEvent->getPath().string().c_str(), 
	       pNewEvent->getMask(),
	       pNewEvent->getMaskString().c_str(),
	       mEventList.size());
    if(!dispatchEvent(pNewEvent, sourceFolder))
      mEventList.pop_back();

  }

}

/**
 * @brief Dispatches events, because there was some error.
 *
 * Dispatching an event means in this case to restart
 * handleEvent and if it fails again, events will be
 * rejected.
 *
 **/
bool EventManager::dispatchEvent(FileSystemEvent* const pEvent, const std::string sourceFolder){
  dbg_printc(LOG_DBG, "EventManager", "DispatchEvent","Dispatch event");
  boost::filesystem::path eventPath = pEvent->getPath();
  if(boost::filesystem::exists(eventPath)){
    return !handleEvent(pEvent, sourceFolder);
  }
  dbg_printc(LOG_DBG, "EventManager","DispatchEvent","Tried to dispatch event, but event was tropped because file is not there anymore");
  return false;

}

/**
 * @brief Can be used to signal new events
 *
 **/
EventManagerSignal EventManager::signalEvent(){
  return mEventManagerSignal;

}



