
#ifndef EventManager_H
#define EventManager_H

#include <vector>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
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
class EventManager{
public:
  EventManager(SyncManager* const pSyncManager);
  void PushBackEvent(inotify_event* const pNewEvent, string sourceFolder);
protected:
  vector<inotify_event*> mEventList;
  SyncManager* const mpSyncManager;
  virtual bool HandleEvent(inotify_event* const pEvent, const string sourceFolder) = 0;


};

#endif /* EventManager_H */
