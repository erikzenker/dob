#include "SyncManager.h"


SyncManager::SyncManager(string destFolder){
  assert(opendir(destFolder.c_str()));
  mDestFolder = destFolder;

}

SyncManager::SyncManager(){

}


