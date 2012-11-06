/*
  UNDER CONSTRUCION
  - StartToScan method
 */

#include "InotifyFileSystemScanner.h"

InotifyFileSystemScanner::InotifyFileSystemScanner(const string scanFolder, EventManager* const pEventManager) 
  : FileSystemScanner(scanFolder, pEventManager ){
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
  Stop();
  cout << "\nC Stop scanning folders";
  return 0;
}

void InotifyFileSystemScanner::Setup(){

}

void InotifyFileSystemScanner::Execute(void* arg){
  Inotify inotify;
  if(!inotify.WatchFolderRecursively(mScanFolder)){
    dbg_print(LOG_ERR,"\nC Failed to watch recursively errno: %d", inotify.GetLastError());

  }

  dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: Start scanning folder: %s", mScanFolder.c_str());
  FileSystemEvent<int> * fileSystemEvent = inotify.GetNextEvent();

  while(fileSystemEvent){
    mpEventManager->PushBackEvent(fileSystemEvent, mScanFolder);

    //Add/delete watches for added/deleted folders or files
    switch(fileSystemEvent->GetMask()){
    case IN_DELETE:
    case IN_DELETE | IN_ISDIR:
      // @todo remove watches
      dbg_print(LOG_DBG, 
		"\nC InotifyFileSystemScanner::Execute: remove watch file: %s no consequenz (TODO)",
		fileSystemEvent->GetFilename().c_str());
      break;

    case IN_CREATE:
    case IN_CREATE | IN_ISDIR:
      // @todo dont rewatch all folders again (needs to much time)
      dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: Add new watch file: %s", fileSystemEvent->GetFilename().c_str());
      inotify.WatchFolderRecursively(mScanFolder);
      break;

    default:
      dbg_print(LOG_ERR, "\nC Unexpected event was triggered %s", fileSystemEvent->GetFilename().c_str());
    }

    // Handle next event
    fileSystemEvent = inotify.GetNextEvent();
  }

}
