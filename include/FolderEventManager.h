
#ifndef FolderEventManager_H
#define FolderEventManager_H

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
 * @brief Handels events from FileSystemScanner
 * @class FolderEventManager
 *        FolderEventManager.h 
 *        "include/FolderEventManager.h"
 *
 * This EventManager is optimized towards the SimpleEventManager
 * because on incoming events not the whole syncFolder will
 * be syncronized, but just the changes. This is faster and
 * more efficient.
 *
 **/
class FolderEventManager : public EventManager{
 public:
  FolderEventManager(SyncManager * pSyncManager);
 private:
  virtual bool handleEvent(FileSystemEvent* pEvent, string sourceFolder);
 
};

#endif /* FolderEventManager_H */
