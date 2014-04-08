#include <boost/filesystem.hpp>

#include <FileEventManager.h>
#include <FileSystemEvent.h>
#include <Inotify.h>


FileEventManager::FileEventManager(SyncManager& syncManager,  const std::vector<std::string> ignoredDirectories) : 
  EventManager(syncManager),
  mIgnoredDirectories(ignoredDirectories){
}

bool FileEventManager::handleEvent(const FileSystemEvent event, const boost::filesystem::path rootPath) const{
  const boost::filesystem::path eventPath = event.path;

  if(isIgnored(eventPath.string())){
    return true;
  }

  switch(event.mask){
  case IN_MODIFY:
    return syncManager.pushFile(rootPath, eventPath);
    break;

  case IN_MOVED_TO:
  case IN_CREATE:
    return syncManager.pushFile(rootPath, eventPath);
    break;
  case IN_CREATE | IN_ISDIR:
    // Delay gives os time to cp data into folder

    usleep(1000000); 
    if(event.isRecursive){
      return syncManager.pushDir(rootPath, eventPath);
    }
    else {
      return syncManager.mkdir(rootPath, eventPath);
    }

    break;

  case IN_MOVED_FROM:
  case IN_DELETE:
    return syncManager.removeFile(rootPath, eventPath);
    break;
  case IN_DELETE | IN_ISDIR:
    return syncManager.removeDir(rootPath, eventPath/"/");
    break;
  default:
    return true;
    break;

  }
  return false;
}


bool FileEventManager::isIgnored(std::string file) const{
  if(mIgnoredDirectories.empty()){
    return false;
  }
 
  for(unsigned i = 0; i < mIgnoredDirectories.size(); ++i){
    size_t pos = file.find(mIgnoredDirectories[i]);
    if(pos!= std::string::npos){
      return true;
    }
  }

  return false;
}


