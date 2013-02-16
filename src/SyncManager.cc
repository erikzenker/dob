#include "SyncManager.h"


SyncManager::SyncManager(string destFolder, string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  
  if(mDestFolder[mDestFolder.size()-1] != '/')
     mDestFolder.append("/");

  setupDestination();

}

 SyncManager::~SyncManager(){

 }

bool SyncManager::setupDestination(){
  return false;
}

