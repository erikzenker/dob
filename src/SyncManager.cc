#include "SyncManager.h"


SyncManager::SyncManager(string destFolder) : 
mDestFolder(destFolder){
  assert(opendir(destFolder.c_str()));

}

SyncManager::SyncManager(){

}


