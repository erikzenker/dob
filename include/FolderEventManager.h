
#ifndef FolderEventManager_H
#define FolderEventManager_H

#include <string>
#include <SyncManager.h>
#include <EventManager.h>
#include <boost/filesystem.hpp>


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
  FolderEventManager(SyncManager * pSyncManager, boost::filesystem::path scanPath);
 private:
  virtual bool handleEvent(FileSystemEvent event, std::string sourceFolder, const bool recursive);
 
};

#endif /* FolderEventManager_H */
