
#pragma once
#include <boost/filesystem.hpp>

#include <SyncManager.h>
#include <EventManager.h>
#include <FileSystemEvent.h>


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
  FileEventManager(SyncManager& syncManager);
 private:
  virtual bool handleEvent(const FileSystemEvent event, const boost::filesystem::path rootPath) const;
 
};


