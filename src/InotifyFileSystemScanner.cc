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
  dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: Start scanning folder: %s", mScanFolder.c_str());
  int events = IN_MODIFY | IN_CREATE | IN_DELETE;

  if ( !inotifytools_initialize()
       || !inotifytools_watch_recursively_follow_symlinks(mScanFolder.c_str(), events)){ 

    dbg_print(LOG_ERR,"\nC Error errno: %d", inotifytools_error());
    //return -1;
  }


   struct inotify_event * event = inotifytools_next_event( -1 );

  while ( event ) {
    mpEventManager->PushBackEvent(event, mScanFolder);


    //Add/delete watches for added/deleted folders or files
    switch(event->mask){
      case IN_DELETE:
	dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: remove watch file: %s no consequenz (TODO)",event->name);
      //inotifytools_initialize();
      //inotifytools_remove_watch_by_wd(event->wd);
      break;
    case IN_DELETE | IN_ISDIR:
      dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: remove watch file: %s no consequenz (TODO)", event->name);
      //inotifytools_initialize();
      //inotifytools_remove_watch_by_wd(event->wd);
      break;
    case IN_CREATE:
      dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: Add new watch file: %s", event->name);
      inotifytools_initialize();
      inotifytools_watch_recursively(mScanFolder.c_str(), events);
      break;
    case IN_CREATE | IN_ISDIR:
      dbg_print(LOG_DBG, "\nC InotifyFileSystemScanner::Execute: Add new watch file: %s", event->name);
      inotifytools_initialize();
      inotifytools_watch_recursively(mScanFolder.c_str(), events);
      break;
    }

    // Handle next event
    event = inotifytools_next_event( -1 );
  }
  //return 0;
}
