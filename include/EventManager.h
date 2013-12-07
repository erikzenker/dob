
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
#include <boost/filesystem.hpp>

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
  EventManager(SyncManager* const pSyncManager, boost::filesystem::path scanPath);
  ~EventManager();
  bool pushBackEvent(FileSystemEvent newEvent, const std::string sourceFolder);

protected:
  bool dispatchEvent(const FileSystemEvent event, const std::string sourceFolder);
  virtual bool handleEvent(const FileSystemEvent event, const std::string sourceFolder) = 0;
  
  /* Member */ 
  std::vector<FileSystemEvent> mEventList;
  SyncManager* const mpSyncManager;

};

#endif /* EventManager_H */
