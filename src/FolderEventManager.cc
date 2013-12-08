#include <FolderEventManager.h>
#include <boost/filesystem.hpp>

FolderEventManager::FolderEventManager(SyncManager * pSyncManager, boost::filesystem::path scanPath)
  : EventManager(pSyncManager, scanPath){
}

bool FolderEventManager::handleEvent(FileSystemEvent event, std::string sourceFolder, const bool recursive){
  boost::filesystem::path eventPath = event.getPath();

  switch(event.getMask()){
  case IN_MODIFY:
    return(mpSyncManager->pushFile(sourceFolder, eventPath));
    break;
  case IN_MOVED_TO:
  case IN_CREATE:
    return(mpSyncManager->pushFile(sourceFolder, eventPath));
    break;
  case IN_CREATE | IN_ISDIR:
    return(mpSyncManager->pushDir(sourceFolder, eventPath, recursive));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->removeDir(sourceFolder, eventPath));
    break;
  default:
    dbg_printc(LOG_ERR, "FolderEventManager", "HandleEvent", "No handler for this event implementet: %s",event.getMaskString().c_str());
    return true;
    break;

  }
  return false;
}


