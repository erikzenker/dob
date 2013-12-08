#include <FileEventManager.h>
#include <boost/filesystem.hpp>
#include <unistd.h>
#include <iostream>

FileEventManager::FileEventManager(SyncManager * pSyncManager, boost::filesystem::path scanPath)
  : EventManager(pSyncManager, scanPath){
}

bool FileEventManager::handleEvent(FileSystemEvent event, string sourceFolder, const bool recursive){
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
    // Delay gives os time to cp data into folder
    usleep(1000000); 
    return(mpSyncManager->pushDir(sourceFolder, eventPath, recursive));
    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
    return(mpSyncManager->removeFile(sourceFolder, eventPath));
    break;
  case IN_DELETE | IN_ISDIR:
    return(mpSyncManager->removeDir(sourceFolder, eventPath/"/"));
    break;
  default:
    dbg_printc(LOG_ERR, "FileEventManager", "HandleEvent", "No handler for this event implementet: %s",event.getMaskString().c_str());
    return true;
    break;

  }
  return false;
}


