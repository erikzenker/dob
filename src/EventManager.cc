#include <EventManager.h>
#include <boost/filesystem.hpp>

EventManager::EventManager(SyncManager * const pSyncManager, boost::filesystem::path scanPath) : 
  mpSyncManager(pSyncManager){

    
}

EventManager::~EventManager(){
  free(mpSyncManager);
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
bool EventManager::pushBackEvent(FileSystemEvent newEvent, const std::string sourceFolder, const bool recursive = true){
  mEventList.push_back(newEvent);
  if(handleEvent(newEvent, sourceFolder, recursive)){
    mEventList.pop_back();
    dbg_printc(LOG_DBG, 
	       "EventManager",
	       "PushBackEvent",
	       "Last event was handled %s %d %s",
	       newEvent.getPath().string().c_str(), 
	       newEvent.getMask(),
	       newEvent.getMaskString().c_str());
    return true;
  }
  else{
    dbg_printc(LOG_WARN, 
    	       "EventManager",
    	       "PushBackEvent",
    	       "Last event was not handled %s %d %s", 
    	       newEvent.getPath().string().c_str(), 
    	       newEvent.getMask(),
    	       newEvent.getMaskString().c_str(),
    	       mEventList.size());
    // if(dispatchEvent(newEvent, sourceFolder))
    //   return true;
    mEventList.pop_back();
  }
  return false;

}

/**
 * @brief Dispatches events, because there was some error.
 *
 * Dispatching an event means in this case to restart
 * handleEvent and if it fails again, events will be
 * rejected.
 *
 **/
bool EventManager::dispatchEvent(FileSystemEvent event, const std::string sourceFolder){
  dbg_printc(LOG_DBG, "EventManager", "DispatchEvent","Dispatch event");
  boost::filesystem::path eventPath = event.getPath();
  if(boost::filesystem::exists(eventPath)){
    return !handleEvent(event, sourceFolder, true);
  }
  dbg_printc(LOG_DBG, "EventManager","DispatchEvent","Tried to dispatch event, but event was tropped because file is not there anymore");
  return false;

}


