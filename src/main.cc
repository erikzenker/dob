/*
 * ROADMAP
 *
 * 1. Copy mechanism (Rsync) --> done
 * 2. Versioning of backup data (Git)
 * 3. Mounting of server data
 * 4. Scanning of server data
 * 6. Ask in case of big data
 * 5. Graphical user interface
 *
 */


// Include Libraries
#include <iostream>
#include <string>
#include "stdlib.h"

// Include Classes
#include "InotifyFileSystemScanner.h"
#include "WebdavSyncManager.h"
#include "EventManager.h"

using namespace std;

int main(){
  void* no_arg = NULL;
  string scan_folder = "/home/erik/OpenDropbox/";
  string dest_folder = "/home/erik/OpenDropboxServer/";

  SyncManager * pSyncManager  = new RemoteSyncManager(dest_folder);
  EventManager * pEventManager = new EventManager(pSyncManager);
  InotifyFileSystemScanner inotifyScanner(scan_folder, pEventManager);

  pSyncManager->SyncSourceFolder(scan_folder);
  inotifyScanner.Start(no_arg);

  while(1){
    system("sleep 1s");
  }

  return 0;
}
