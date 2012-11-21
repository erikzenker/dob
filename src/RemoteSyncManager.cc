#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder, string syncType):
  SyncManager(destFolder, syncType){

}

bool RemoteSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  if(CheckDestFolder()){
    string cp_query = "cp -RLv ";  
    cp_query
      .append(syncFolder)
      .append(folder)
      .append(" ")
      .append(mDestFolder)
      .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
      .append(" ");
  
    if(system(cp_query.c_str())){
      dbg_printc(LOG_ERR, "RemoteSyncManager","SyncFolder","Can't reach destination folder, maybe location is offline");
      return false;
    }
    dbg_printc(LOG_DBG,"RemoteSyncManager","SyncFolder","%s ", cp_query.c_str());
  }
  else{
    dbg_printc(LOG_ERR, "RemoteSyncManager","SyncFolder","Failed syncronise source and destination folder, because destination folder is not mounted");

  }
  return true;
  
}

bool RemoteSyncManager::SyncFile(string sourceFolder, string syncFolder){
  return true;

}

bool RemoteSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  string rm_query = "rm -Rv ";
  rm_query
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
    .append(folder);

  if(system(rm_query.c_str())){
    dbg_printc(LOG_ERR, "RemoteSyncManager","RemoveFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG, "RemoteSyncManager","RemoveFolder","%s", rm_query.c_str());
  return true;

}

bool RemoteSyncManager::CheckDestFolder(){
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);
  return !system(mountpoint_query.c_str());
}

bool RemoteSyncManager::MountDestFolder(){
  return false;
}
