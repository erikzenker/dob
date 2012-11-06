
#ifndef OptimizedEventManager_H
#define OptimizedEventManager_H

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <SyncManager.h>
#include <EventManager.h>
#include <dbg_print.h>

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
  bool HandleEvent(FileSystemEvent<int>* pEvent, string sourceFolder);
public:
  SimpleEventManager(SyncManager * pSyncManager);
 
};

#endif /* SimpleEventManager_H */
