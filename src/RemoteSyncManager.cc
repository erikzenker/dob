#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder, string syncType, string mountOptions):
  SyncManager(destFolder, syncType),
  mMountOptions(mountOptions){

}

/**
 *  @todo check mMountOptions, sourcefolder for "\" as last char
 *
 **/
bool RemoteSyncManager::SyncSourceFolder(string sourceFolder){
  dbg_printc(LOG_DBG, "RemoteSyncManager","SyncSourceFolder", "Syncronise source and destination folder");
  string rsync_push_query = "rsync -vzruLKpt ";
  string rsync_pull_query = "rsync -vzruLKpt ";
  rsync_push_query
    .append(sourceFolder)
    .append(" ")
    .append(mMountOptions);
  rsync_pull_query
    .append(mMountOptions)      
    .append(" ")
    .append(sourceFolder);
  cerr << "\n";
  dbg_printc(LOG_DBG, "RemoteSyncManager", "SyncSourceFolder", "Rsync pull string: %s", rsync_pull_query.c_str());
  dbg_printc(LOG_DBG, "RemoteSyncManager", "SyncSourceFolder", "Rsync push string: %s", rsync_push_query.c_str());
  system(rsync_pull_query.c_str());
  system(rsync_push_query.c_str());

  return true;
}

bool RemoteSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  string cp_query = "rsync -vzruLKptD ";  
  cp_query
    .append(syncFolder)
    .append(folder)
    .append(" ")
    .append(mMountOptions)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
    .append(" ");
  
  if(system(cp_query.c_str())){
    dbg_printc(LOG_ERR, "RemoteSyncManager","SyncFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG,"RemoteSyncManager","SyncFolder","%s ", cp_query.c_str());

  return true;
  
}

bool RemoteSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  /*
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
  */
  return false;

}

bool RemoteSyncManager::CheckDestFolder(){
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);
  return !system(mountpoint_query.c_str());
}

bool RemoteSyncManager::MountDestFolder(){
  if(!mMountOptions.compare(""))
    return false;

  string mount_query;
  mount_query
    .append("sshfs -o ServerAliveInterval=15 ")
    .append(mMountOptions)
    .append(" ")
    .append(mDestFolder);
  cerr << mount_query << "\n";
  system(mount_query.c_str());

  return true;
}
