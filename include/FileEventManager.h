
#ifndef FileEventManager_H
#define FileEventManager_H

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
 * @brief Handels events from FileSystemScanner on
 *        file level.
 * @class FileEventManager
 *        FileEventManager.h 
 *        "include/FileEventManager.h"
 *
 * This EventManager handles every Eventfile
 * for its own. In difference to OptimizedEventManager
 * where whole folders are handled.
 *
 **/
class FileEventManager : public EventManager{
 public:
  FileEventManager(SyncManager * pSyncManager);
 private:
  virtual bool handleEvent(FileSystemEvent<int>* pEvent, string sourceFolder);
 
};

#endif /* FileEventManager_H */