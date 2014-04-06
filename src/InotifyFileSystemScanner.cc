#include "InotifyFileSystemScanner.h"

#include <vector>
#include <string>
#include <sys/inotify.h>
#include <boost/filesystem.hpp>

#include <FileSystemScanner.h>
#include <Inotify.h>
#include <FileSystemEvent.h>
#include <FileStateDatabase.h>
#include <FileState.h>

InotifyFileSystemScanner::InotifyFileSystemScanner(const boost::filesystem::path rootPath, const EventManager& eventManager, const std::vector<std::string> ignoredFolders, const unsigned eventTimeout, const std::string profileName) : 
  FileSystemScanner(rootPath, eventManager),
  _inotify(ignoredFolders, eventTimeout, IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE),
  _fileStateDatabase(ignoredFolders,"." + profileName + "_statedb.sql", rootPath){

}

InotifyFileSystemScanner::~InotifyFileSystemScanner(){
  
}

/**
 * @brief Starts to scan the rootPath for 
 *        events on the filesystem.
 *
 **/
void InotifyFileSystemScanner::start(){
  // Handle updates since last call
  for(auto update: _fileStateDatabase.updates()){
    FileSystemEvent fileSystemEvent = toFileSystemEvent(update);

    if(eventManager.handleEvent(fileSystemEvent, rootPath)){
      _fileStateDatabase.propagateUpdate(update);
    }
    
  }

  // Create file watches
  try{
    _inotify.watchDirectoryRecursively(rootPath);
  }
  catch(std::runtime_error e){
    std::cerr << e.what() << std::endl;
  }
  

  while(true){
    FileSystemEvent fileSystemEvent = _inotify.getNextEvent();
    fileSystemEvent.isRecursive = true;

    // In case file was deleted, need to restore FileState 
    FileState fs = _fileStateDatabase.getFileState(fileSystemEvent.path);
    if(fs.is_dir){
      fileSystemEvent.mask = fileSystemEvent.mask | IN_ISDIR;
    }

    // Handle event
    bool result = eventManager.handleEvent(fileSystemEvent, rootPath);

    if(result){
      // Add or delete watches for added/deleted folders or files
      switch(fileSystemEvent.mask){
      case IN_MOVED_FROM:
      case IN_DELETE:
      case IN_DELETE | IN_ISDIR:
  	_fileStateDatabase.propagateUpdateRecursively(fileSystemEvent.path, FS_DELETE);
  	// @todo remove watches recursively
  	// @bug removing watches also removes information
  	//      about the location of a watch
  	// @todo think about this situation
  	//       can be solved by FileStateDatabase
  	//mInotify->watchFile(fileSystemEvent->getFolderPath());
  	//mInotify->removeWatch(fileSystemEvent->getId());
  	break;

      case IN_MOVED_TO:
      case IN_CREATE:
      case IN_CREATE | IN_ISDIR:
	try{
	  _inotify.watchDirectoryRecursively(fileSystemEvent.path);
	}
	catch(std::runtime_error e){
	  std::cerr << e.what() << std::endl;
	  break;
	}
	catch(std::invalid_argument e){
	  std::cerr << e.what() << std::endl;
	  break;
	}
  	_fileStateDatabase.propagateUpdateRecursively(fileSystemEvent.path, FS_CREATE);
  	break;

      case IN_MODIFY:
  	_fileStateDatabase.propagateUpdateRecursively(fileSystemEvent.path, FS_MODIFY);
  	break;

      default:
	break;
  	
      }

    }

  }
      
}

void InotifyFileSystemScanner::stop(){

}

FileSystemEvent InotifyFileSystemScanner::toFileSystemEvent(std::pair<FileState, ModState> update){
  FileState fs = update.first;
  ModState ms  = update.second;

  bool is_dir = fs.is_dir;
  boost::filesystem::path path = fs.path;
  uint32_t mask = 0;

  // Convert modstate to inotify mask
  if(is_dir)
    mask |= IN_ISDIR;

  switch(ms){

  case FS_MODIFY:
    mask |= IN_MODIFY;
    break;

  case FS_CREATE:
    mask |= IN_CREATE;
    break;

  case FS_DELETE:
    mask |= IN_DELETE;
    break;
  default:
    break;
  };

  return FileSystemEvent(0, mask, path);

}
