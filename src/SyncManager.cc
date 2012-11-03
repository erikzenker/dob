#include "SyncManager.h"


SyncManager::SyncManager(string destFolder, string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  assert(opendir(destFolder.c_str()));

}

SyncManager::SyncManager(){

}


