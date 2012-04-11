#include <iostream>
#include <vector>
#include <stdio.h>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>

#include "Thread.h"
#include "FileSystemScanner.h"
#include "EventManager.h"

class InotifyFileSystemScanner : public FileSystemScanner,public Thread {
private:
  EventManager* mEventManager;
public:
  InotifyFileSystemScanner(string scan_folder);
  int StartToScan();
  int StopToScan();
  void Setup();
  void Execute(void* arg);

};
