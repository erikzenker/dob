#include <SyncManager.h>
#include <SyncType.h>

SyncManager::SyncManager(std::string destFolder, SyncType syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  
  if(mDestFolder[mDestFolder.size()-1] != '/')
     mDestFolder.append("/");

}

 SyncManager::~SyncManager(){

 }


