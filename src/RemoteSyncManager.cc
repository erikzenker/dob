#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder, string syncType, string destProtocol):
  SyncManager(destFolder, syncType),
  mDestProtocol(destProtocol){

}

/**
 *  @todo check destFolder, sourcefolder for "\" as last char
 *
 **/
bool RemoteSyncManager::SyncSourceFolder(string sourceFolder){
  dbg_printc(LOG_DBG, "RemoteSyncManager","SyncSourceFolder", "Syncronise source and destination folder");
  string rsync_push_query = "rsync -vzruLKpt ";
  string rsync_pull_query = "rsync -vzruLKpt ";
  rsync_push_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestFolder);
  rsync_pull_query
    .append(mDestFolder)      
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
  string rsync_query = "rsync -vzruLKpt ";  
  rsync_query
    .append(syncFolder)
    //.append(folder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));
  
  if(system(rsync_query.c_str())){
    dbg_printc(LOG_ERR, "RemoteSyncManager","SyncFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG,"RemoteSyncManager","SyncFolder","%s ", rsync_query.c_str());

  return true;
  
}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool RemoteSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  
  string rm_query = "rsync -vzruLKpt --delete ";
  rm_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  if(system(rm_query.c_str())){
    dbg_printc(LOG_ERR, "RemoteSyncManager","RemoveFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG, "RemoteSyncManager","RemoveFolder","%s", rm_query.c_str());
  return true;
  
  return false;

}

/**
 * @todo fill with content
 *
 **/
bool RemoteSyncManager::CheckDestFolder(){
  /*
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);
  return !system(mountpoint_query.c_str());
  */
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool RemoteSyncManager::MountDestFolder(){
  /*
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
  */
  return false;
}
