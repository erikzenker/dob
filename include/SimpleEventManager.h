
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

/**
 *
 * @brief Handels events from FileSystemScanner in a simple way
 * @class SimpleEventManager
 *        SimpleEventManager.h 
 *        "include/SimpleEventManager.h"
 *
 **/
class SimpleEventManager : public EventManager{
private:
  bool HandleEvent(inotify_event* pEvent, string sourceFolder);
public:
  SimpleEventManager(SyncManager * pSyncManager);
 
};

#endif /* SimpleEventManager_H */
