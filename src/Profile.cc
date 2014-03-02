#include <thread>

#include <Profile.h>
#include <FileSystemScanner.h>
#include <SyncType.h>

Profile::Profile(FileSystemScanner& fileSystemScanner, EventManager& eventManager, SyncManager& syncManager) : 
  fileSystemScanner(fileSystemScanner),
  eventManager(eventManager),
  syncManager(syncManager){
 
}

void Profile::start(){
	std::thread thread(&FileSystemScanner::start, &fileSystemScanner);
	thread.join();
}

void Profile::stop(){
 
}

