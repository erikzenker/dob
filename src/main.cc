// Include Libraries
#include <iostream>
#include <string>

// Include Classes
#include "InotifySystemScanner.h"
#include "stdlib.h"

using namespace std;


int main(){
  void* no_arg = NULL;
  string scan_folder = "/home/erik/OpenDropbox/";
  string dest_folder = "/home/erik/OpenDropboxServer/";

  SyncManager * pSyncManager  = new SyncManager(dest_folder);
  EventManager * pEventManager = new EventManager(pSyncManager);
  InotifyFileSystemScanner inotifyScanner(scan_folder, pEventManager);

  pSyncManager->SyncSourceFolder(scan_folder);
  inotifyScanner.Start(no_arg);

  while(1){
    system("sleep 1s");
  }

  return 0;
}
