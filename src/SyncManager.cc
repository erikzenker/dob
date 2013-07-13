#include "SyncManager.h"


SyncManager::SyncManager(std::string destFolder, std::string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  
  if(mDestFolder[mDestFolder.size()-1] != '/')
     mDestFolder.append("/");

}

 SyncManager::~SyncManager(){

 }


