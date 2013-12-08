#include "InotifyFileSystemScanner.h"

#include <vector>
#include <string>
#include <sys/inotify.h>

#include <FileSystemScanner.h>
#include <dbg_print.h>
#include <Inotify.h>
#include <FileSystemEvent.h>
#include <Profile.h>
#include <FileStateDatabase.h>
#include <FileState.h>
#include <boost/filesystem.hpp>

InotifyFileSystemScanner::InotifyFileSystemScanner(const std::string scanFolder, std::vector<std::string> ignoredFolders, const int eventTimeout, EventManager* const pEventManager, const std::string profileName) : 
  FileSystemScanner(scanFolder, pEventManager),
  mInotify(ignoredFolders, eventTimeout, IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE),
  mFileStateDatabase("." + profileName + "_statedb.sql", scanFolder){

}

InotifyFileSystemScanner::~InotifyFileSystemScanner(){
  
}

int InotifyFileSystemScanner::startToScan(){
  return start(NULL);
}

int InotifyFileSystemScanner::stopToScan(){
  return stop();
}

void InotifyFileSystemScanner::setup(){
  
}

/**
 * @brief Threaded scanning of scanFolder
 *
 * With help of inotify the scanFolder will be watched
 * recursively for events. If events occur they will
 * be pushed to an eventManager. New folders/files will 
 * also be watched again or removed from watches.
 *
 * @todo when deleting a symlink from a watched folder, then
 *       the folder the symlink points to should be removed
 *       from watched folders.
 *       IDEA: save symlinks in kind of database so you
 *             know the pointed folder
 *	 Handle more then one event at a time
 * 
 **/
void InotifyFileSystemScanner::execute(void* arg){
  // Handle updates since last call
  dbg_printc(LOG_DBG,"InotifyFileSystemScanner", "Execute", "Initial sync");
  auto updates = mFileStateDatabase.updates();
  for(auto update = updates.begin(); update != updates.end(); ++update){
    auto fileSystemEvent = toFileSystemEvent(*update);
    FileState fs = update->first;
    bool recursive = false;
    bool result = mpEventManager->pushBackEvent(fileSystemEvent, mScanFolder, recursive);
    if(result)
      mFileStateDatabase.propagateUpdate(*update, recursive);
    
  }

  // Create file watches
  if(!mInotify.watchDirectoryRecursively(mScanFolder)){
    dbg_printc(LOG_ERR,"InotifyFileSystemScanner", "Execute", "Failed to watch recursively errno: %d", mInotify.getLastErrno());

  }

  // Update changed files to server
  dbg_printc(LOG_DBG,"InotifyFileSystemScanner", "Execute", "Start scanning folder: %s", mScanFolder.c_str());

  while(true){
    bool recursive = true;
    FileSystemEvent fileSystemEvent = mInotify.getNextEvent();
    dbg_printc(LOG_DBG, 
  	       "FileSystemEvent",
  	       "execute",
  	       "New event %s %d %s",
  	       fileSystemEvent.getPath().string().c_str(), 
  	       fileSystemEvent.getMask(),
  	       fileSystemEvent.getMaskString().c_str());


    // In case file was deleted, need to get FileState
    FileState fs = mFileStateDatabase.getFileState(fileSystemEvent.getPath());
    if(fs.is_dir){
      fileSystemEvent.setMask(fileSystemEvent.getMask() | IN_ISDIR);
    }

    // Handle event by eventmanager
    bool result = mpEventManager->pushBackEvent(fileSystemEvent, mScanFolder, recursive);

    if(result){
      // Add or delete watches for added/deleted folders or files
      switch(fileSystemEvent.getMask()){
      case IN_MOVED_FROM:
      case IN_DELETE:
      case IN_DELETE | IN_ISDIR:
  	mFileStateDatabase.propagateUpdate(fileSystemEvent.getPath(), FS_DELETE, recursive);
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
  	dbg_printc(LOG_DBG, "InotifyFileSystemScanner", "Execute", "Add new watch file: %s", fileSystemEvent.getPath().string().c_str());
  	mInotify.watchDirectoryRecursively(fileSystemEvent.getPath().string());
  	propagateUpdateRecursive(fileSystemEvent.getPath(), FS_CREATE);
  	break;

      case IN_MODIFY:
  	propagateUpdateRecursive(fileSystemEvent.getPath(), FS_MODIFY);
  	break;
      default:
  	dbg_printc(LOG_ERR, 
  		   "InotifyFileSystemScanner", 
  		   "Execute",
  		   "Unexpected event was triggered %s %s",fileSystemEvent.getMaskString().c_str(), fileSystemEvent.getPath().string().c_str());
      }

    }

  }
      
}

void InotifyFileSystemScanner::propagateUpdateRecursive(const boost::filesystem::path rootPath, const ModState ms){
  boost::filesystem::recursive_directory_iterator it(rootPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  mFileStateDatabase.propagateUpdate(rootPath, ms, true);

  while(it != end){
    bool recursive = true;
    boost::filesystem::path currentPath = ((boost::filesystem::path)*it);
    mFileStateDatabase.propagateUpdate(currentPath, ms, recursive);
    ++it;
  }

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
