
#ifndef EventManager_H
#define EventManager_H

#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/inotify.h>
#include <sigc++/sigc++.h>
#include <SyncManager.h>
#include <dbg_print.h>
#include <FileSystemEvent.h>

/**
 * @brief Handels events from FileSystemScanner
 * @class EventManager
 *        EventManager.h 
 *        "include/EventManager.h"
 *
 * The EvenManager takes different events 
 * [pushBackEvent(FileSystemEvent)] from a 
 * FileSystemScanner and tries handel them (HandleEvent).
 * 
 *
 **/

class EventManager{
public:
  EventManager(SyncManager* const pSyncManager);
  ~EventManager();
  void pushBackEvent(FileSystemEvent* const pNewEvent, const std::string sourceFolder);

protected:
  bool dispatchEvent(FileSystemEvent* const pEvent, const std::string sourceFolder);
  virtual bool handleEvent(FileSystemEvent* const pEvent, const std::string sourceFolder) = 0;
  
  /* Member */ 
  std::vector<FileSystemEvent* > mEventList;
  SyncManager* const mpSyncManager;

};

#endif /* EventManager_H */
