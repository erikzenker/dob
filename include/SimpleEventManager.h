/** 
 * @file      SimpleEventManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

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
 * @brief Handels events from FileSystemScanner in a simple way.
 * @class SimpleEventManager
 *        SimpleEventManager.h 
 *        "include/SimpleEventManager.h"
 *
 * Simple means in this case that for each event will be done
 * a full syncroniation between source and destination folder.
 * In the most cases this is not necessary, because it is faster
 * just copying files.
 *
 **/
class SimpleEventManager : public EventManager{
private:
  bool handleEvent(FileSystemEvent<int>* pEvent, string sourceFolder);
public:
  SimpleEventManager(SyncManager * pSyncManager);
 
};

#endif /* SimpleEventManager_H */
