/*
  UNDER CONSTRUCION
  - StartToScan method
 */

#include "InotifyFileSystemScanner.h"

InotifyFileSystemScanner::InotifyFileSystemScanner(const string scanFolder, EventManager* const pEventManager) 
  : FileSystemScanner(scanFolder, pEventManager ),
    mIgnoredFolder(""){
}

InotifyFileSystemScanner::InotifyFileSystemScanner(const string scanFolder, const string ignoredFolder, EventManager* const pEventManager) 
  : FileSystemScanner(scanFolder, pEventManager ),
    mIgnoredFolder(ignoredFolder){

}

/**
 * @todo when deleting a symlink from a watched folder, then
 *       the folder the symlink points to should be removed
 *       from watched folders.
 *       IDEA: save symlinks in kind of database so you
 *             know the pointed folder
 *	 Handle more then one event at a time
 * 
 * Usefull function
 *
 * canonicalize_file_name ("/home/erik/OpenDropbox/Mail");
 * http://www.gnu.org/software/libc/manual/html_node/Symbolic-Links.html
 **/
int InotifyFileSystemScanner::StartToScan(){
  return Start(NULL);
}

int InotifyFileSystemScanner::StopToScan(){
  return Stop();
}

void InotifyFileSystemScanner::Setup(){

}

void InotifyFileSystemScanner::Execute(void* arg){
  Inotify inotify(mIgnoredFolder);
  if(!inotify.WatchFolderRecursively(mScanFolder)){
    dbg_printc(LOG_ERR,"InotifyFileSystemScanner", "Execute", "Failed to watch recursively errno: %d", inotify.GetLastError());

  }

  dbg_printc(LOG_DBG,"InotifyFileSystemScanner", "Execute", "Start scanning folder: %s", mScanFolder.c_str());
  FileSystemEvent<int> * fileSystemEvent = inotify.GetNextEvent();

  while(fileSystemEvent){
    mpEventManager->PushBackEvent(fileSystemEvent, mScanFolder);

    //Add/delete watches for added/deleted folders or files
    switch(fileSystemEvent->GetMask()){
    case IN_MOVED_FROM:
    case IN_DELETE:
    case IN_DELETE | IN_ISDIR:
      // @todo remove watches recursively
      dbg_printc(LOG_DBG, "InotifyFileSystemScanner", "Execute",
		"Remove watch file: %s no consequenz (TODO)",
		fileSystemEvent->GetFilename().c_str());
      break;

    case IN_MOVED_TO:
    case IN_CREATE:
    case IN_CREATE | IN_ISDIR:
      dbg_printc(LOG_DBG, "InotifyFileSystemScanner", "Execute", "Add new watch file: %s", fileSystemEvent->GetFilename().c_str());
      inotify.WatchFolderRecursively(fileSystemEvent->GetFullPath());
      break;

    case IN_MODIFY:
      // Do nothing
      break;
    default:
      dbg_printc(LOG_ERR, 
		 "InotifyFileSystemScanner", 
		 "Execute",
		 "Unexpected event was triggered %s %s",fileSystemEvent->GetMaskString().c_str(), fileSystemEvent->GetFilename().c_str());
    }

    // Handle next event
    fileSystemEvent = inotify.GetNextEvent();
  }

}
