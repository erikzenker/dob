
#ifndef OptimizedEventManager_H
#define OptimizedEventManager_H

#include <vector>
#include <iostream>
#include <string>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <stdlib.h>
#include <SyncManager.h>
#include <EventManager.h>
#include <dbg_print.h>

using namespace std;

/**
 *
 * @brief Handels events from FileSystemScanner
 * @class OptimizedEventManager
 *        OptimizedEventManager.h 
 *        "include/OptimizedEventManager.h"
 *
 * This EventManager is optimized towards the SimpleEventManager
 * because on incoming events not the whole syncFolder will
 * be syncronized, but just the changes. This is faster and
 * more efficient.
 *
 **/
class OptimizedEventManager : public EventManager{
 public:
  OptimizedEventManager(SyncManager * pSyncManager);
 private:
  bool HandleEvent(inotify_event* pEvent, string sourceFolder);
 
};

#endif /* OptimizedEventManager_H */
