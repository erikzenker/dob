#include <SyncManager.h>
#include <SyncType.h>

SyncManager::SyncManager(boost::filesystem::path destPath, SyncType syncType) : 
  destPath(destPath),
  syncType(syncType){
  
  destPath / "/";

}

 SyncManager::~SyncManager(){

 }

