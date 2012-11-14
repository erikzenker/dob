
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

using namespace std;

/**
 * @brief Handels events from FileSystemScanner
 * @class EventManager
 *        EventManager.h 
 *        "include/EventManager.h"
 *
 * The EvenManager takes different events (FileSystemEvent)
 * from an FileSystemScanner and handels them (HandleEvent).
 *
 **/
typedef sigc::signal<void, bool, int> EventManagerSignal;

class EventManager{
public:
  EventManager(SyncManager* const pSyncManager);
  SyncManager* GetSyncManager() const;
  void PushBackEvent(FileSystemEvent<int>* const pNewEvent, string sourceFolder);
  EventManagerSignal SignalEvent();

protected:
  bool DispatchEvent(FileSystemEvent<int>* const pEvent, const string sourceFolder);
  virtual bool HandleEvent(FileSystemEvent<int>* const pEvent, const string sourceFolder) = 0;
  void SetPauseIcon() const;
  void SetSyncIcon() const;
  void SetScanIcon() const;
  
  /* Member */ 
  vector<FileSystemEvent<int>* > mEventList;
  SyncManager* const mpSyncManager;
  EventManagerSignal mEventManagerSignal;

};

#endif /* EventManager_H */
