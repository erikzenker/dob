#include <SyncManager.h>
#include <SyncType.h>

SyncManager::SyncManager(boost::filesystem::path destPath, SyncType syncType) : 
  mDestPath(destPath),
  mSyncType(syncType){
  
  mDestPath / "/";

}

 SyncManager::~SyncManager(){

 }


