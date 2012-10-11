
#ifndef EventManager_H
#define EventManager_H

#include <vector>
#include <fstream>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <sigc++/sigc++.h>
#include "SyncManager.h"

using namespace std;

/**
 * @brief Handels events from FileSystemScanner
 * @class EventManager
 *        EventManager.h 
 *        "include/EventManager.h"
 *
 * The EvenManager takes different events (inotify_event)
 * from an FileSystemScanner and handels them (HandleEvent).
 *
 *  struct inotify_event {
 *    int      wd;        Watch descriptor 
 *    uint32_t mask;      Mask of events 
 *    uint32_t cookie;    Unique cookie associating related
 *                        events (for rename(2)) 
 *    uint32_t len;       Size of name field 
 *    char     name[];    Optional null-terminated name 
 *  };
 * 
 **/
typedef sigc::signal<void, bool, int> EventManagerSignal;

class EventManager{
public:
  EventManager(SyncManager* const pSyncManager);
  SyncManager* GetSyncManager() const;
  void PushBackEvent(inotify_event* const pNewEvent, string sourceFolder);
  EventManagerSignal SignalEvent();

  /* Member */


protected:
  bool DispatchEvent(inotify_event* const pEvent, const string sourceFolder);
  virtual bool HandleEvent(inotify_event* const pEvent, const string sourceFolder) = 0;
  void SetPauseIcon() const;
  void SetSyncIcon() const;
  void SetScanIcon() const;
  
  /* Member */ 
  vector<inotify_event*> mEventList;
  SyncManager* const mpSyncManager;
  EventManagerSignal mEventManagerSignal;

};

#endif /* EventManager_H */
