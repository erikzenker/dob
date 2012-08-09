
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
 * @class SimpleEventManager
 *        SimpleEventManager.h 
 *        "include/SimpleEventManager.h"
 * @inher protected:
 *          vector<inotify_event*> mEventList;
 *          SyncManager* mpSyncManager;
 *        public:
 *          void PushBackEvent(inotify_event* pNewEvent, string sourceFolder);
 *
 ***/
class SimpleEventManager : public EventManager{
private:
  void HandleEvent(inotify_event* pEvent, string sourceFolder);
public:
  SimpleEventManager(SyncManager * pSyncManager);
 
};

#endif /* SimpleEventManager_H */
