#include "InotifySystemScanner.h"

InotifyFileSystemScanner::InotifyFileSystemScanner(string scan_folder) : FileSystemScanner(scan_folder){
}

int InotifyFileSystemScanner::StartToScan(){
  cerr << "\nC Start scanning folders";
  int events = IN_MODIFY | IN_CREATE | IN_DELETE;
  if ( !inotifytools_initialize()
       || !inotifytools_watch_recursively( "/home/erik/OpenDropbox",events)){ 
	
    fprintf(stderr, "%s\n", inotifytools_error());
    return -1;
  }

  // set time format to 24 hour time, HH:MM:SS
  inotifytools_set_printf_timefmt( "%T" );

  // Output all events as "<timestamp> <path> <events>"
  struct inotify_event * event = inotifytools_next_event( -1 );
  while ( event ) {
    mEventManager->PushBackEvent(event);
    event = inotifytools_next_event( -1 );

  }

  return 0;
}

int InotifyFileSystemScanner::StopToScan(){
  cout << "\nC Stop scanning folders";
  return 0;
}

void InotifyFileSystemScanner::Setup(){
  mEventManager = new EventManager();
}

void InotifyFileSystemScanner::Execute(void* arg){
  this->StartToScan();
  
}
