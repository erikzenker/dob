/*
  UNDER CONSTRUCION
  - StartToScan method
 */

#include "InotifyFileSystemScanner.h"

InotifyFileSystemScanner::InotifyFileSystemScanner(string scanFolder, EventManager* pEventManager) 
  : FileSystemScanner(scanFolder, pEventManager ){
}

/*
 * BUGS
 *
 * Failure on inotifytools_watch_recursively and a
 * following seg fault on inotifytools_error()
 * function. The reason now not known
 *
 * TODO
 *
 * when deleting a symlink from a watched folder, then
 * the folder the symlink points to should be removed
 * from watched folders.
 * IDEA: save symlinks in kind of database so you
 *       know the pointed folder
 *	    
 * Usefull function
 *
 * canonicalize_file_name ("/home/erik/OpenDropbox/Mail");
 * http://www.gnu.org/software/libc/manual/html_node/Symbolic-Links.html
 */
int InotifyFileSystemScanner::StartToScan(){
  cerr << "\nC Start scanning folders";
  int events = IN_MODIFY | IN_CREATE | IN_DELETE;
  if ( !inotifytools_initialize()
       || !inotifytools_watch_recursively(mScanFolder.c_str(), events)){ 

    fprintf(stderr,"\nC Error errno: %d", inotifytools_error());
    return -1;
  }


  struct inotify_event * event = inotifytools_next_event( -1 );

  while ( event ) {
    mpEventManager->PushBackEvent(event, mScanFolder);


    //Add/delete watches for added/deleted folders or files

    switch(event->mask){
      case IN_DELETE:
	cerr << "\nC remove watch file: " << event->name << " no consequenz (TODO)";
      //inotifytools_initialize();
      //inotifytools_remove_watch_by_wd(event->wd);
      break;
    case IN_DELETE | IN_ISDIR:
      cerr << "\nC remove watch folder: " << event->name << " no cosequenz (TODO)";
      //inotifytools_initialize();
      //inotifytools_remove_watch_by_wd(event->wd);
      break;
    case IN_CREATE:
      cerr << "\nC Add new watch file: " << event->name;
      inotifytools_initialize();
      inotifytools_watch_recursively(mScanFolder.c_str(), events);
      break;
    case IN_CREATE | IN_ISDIR:
      cerr << "\nC Add new watch folder: " << event->name;
      inotifytools_initialize();
      inotifytools_watch_recursively(mScanFolder.c_str(), events);
      break;
    }

    // Handle next event
    event = inotifytools_next_event( -1 );
  }

  return 0;
}

int InotifyFileSystemScanner::StopToScan(){
  cout << "\nC Stop scanning folders";
  return 0;
}

void InotifyFileSystemScanner::Setup(){
}

void InotifyFileSystemScanner::Execute(void* arg){
  StartToScan();
  
}
