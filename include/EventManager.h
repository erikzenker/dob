

#pragma once
#include <vector>
#include <boost/filesystem.hpp>

#include <SyncManager.h>
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
  EventManager(const SyncManager& syncManager);
  ~EventManager();
  virtual bool handleEvent(const FileSystemEvent event, const boost::filesystem::path rootPath) const = 0;

protected:
  const SyncManager& syncManager;

};


