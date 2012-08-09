
#ifndef OptimizedEventManager_H
#define OptimizedEventManager_H

#include <vector>
#include <iostream>
#include <string>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <stdlib.h>
#include "SyncManager.h"
#include "EventManager.h"

using namespace std;

/***
 *
 * @brief Handels simple events from FileSystemScanner
 * @class OptimizedEventManager
 *        OptimizedEventManager.h 
 *        "include/OptimizedEventManager.h"
 * @inher protected:
 *          vector<inotify_event*> mEventList;
 *          SyncManager* mpSyncManager;
 *        public:
 *          void PushBackEvent(inotify_event* pNewEvent, string sourceFolder);
 *
 ***/
class OptimizedEventManager : public EventManager{
private:
  void HandleEvent(inotify_event* pEvent, string sourceFolder);
public:
  OptimizedEventManager(SyncManager * pSyncManager);
 
};

#endif /* OptimizedEventManager_H */
