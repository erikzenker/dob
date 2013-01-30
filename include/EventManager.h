
#ifndef EventManager_H
#define EventManager_H

#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/inotify.h>
#include <sigc++/sigc++.h>
#include <SyncManager.h>
#include <dbg_print.h>
#include <FileSystemEvent.h>

/**
 * @brief Handels events from FileSystemScanner
 * @class EventManager
 *        EventManager.h 
 *        "include/EventManager.h"
 *
 * The EvenManager takes different events 
 * [pushBackEvent(FileSystemEvent)] from a 
 * FileSystemScanner and tries handel them (HandleEvent).
 * 
 *
 **/
typedef sigc::signal<void, bool, int> EventManagerSignal;

class EventManager{
public:
  EventManager(SyncManager* const pSyncManager);
  ~EventManager();
  SyncManager* getSyncManager() const;
  void pushBackEvent(FileSystemEvent<int>* const pNewEvent, const std::string sourceFolder);
  EventManagerSignal signalEvent();

protected:
  bool dispatchEvent(FileSystemEvent<int>* const pEvent, const std::string sourceFolder);
  virtual bool handleEvent(FileSystemEvent<int>* const pEvent, const std::string sourceFolder) = 0;
  void setPauseIcon() const;
  void setSyncIcon() const;
  void setScanIcon() const;
  
  /* Member */ 
  vector<FileSystemEvent<int>* > mEventList;
  SyncManager* const mpSyncManager;
  EventManagerSignal mEventManagerSignal;

};

#endif /* EventManager_H */
