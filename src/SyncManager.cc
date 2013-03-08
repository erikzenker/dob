#include "SyncManager.h"


SyncManager::SyncManager(string destFolder, string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  
  if(mDestFolder[mDestFolder.size()-1] != '/')
     mDestFolder.append("/");

}

 SyncManager::~SyncManager(){

 }


