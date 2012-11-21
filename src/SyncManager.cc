#include "SyncManager.h"


SyncManager::SyncManager(string destFolder, string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  assert(opendir(destFolder.c_str()));

}

SyncManager::SyncManager(){

}

/*
 * @note  Rsync command line parameters
 *        -r, --recursive   recurse into directories
 *        -u, --update      skip files that are newer on the receiver
 *        -z, --compress    compress file data during the transfer
 *        -v, --verbose     increase verbosity
 *            --delete      delete extraneous files from dest dirs
 *        -L, --copy-links  transform symlink into referent file/dir
 *        See more in "man rsync"
 */
bool SyncManager::SyncSourceFolder(string sourceFolder){
  if(!CheckDestFolder()){
    if(!MountDestFolder()){
      dbg_printc(LOG_ERR,"RemoteSyncManager","SyncSourceFolder", "Failed to mount destination folder, abort syncronisation");
      return false;
    }
    // Check again just to be shure
    if(!CheckDestFolder()){
      dbg_printc(LOG_ERR,"RemoteSyncManager","SyncSourceFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
      return false;
    }
  }

  dbg_print(LOG_DBG, "\nC RemoteSyncManager::SyncSourceFolder: Syncronise source and destination folder");
  string rsync_push_query = "rsync -vzruL ";
  string rsync_pull_query = "rsync -vzruL ";
  rsync_push_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestFolder);
  rsync_pull_query
    .append(mDestFolder)      
    .append(" ")
    .append(sourceFolder);
  cerr << "\n";
  system(rsync_pull_query.c_str());
  system(rsync_push_query.c_str());

  return true;
}

bool SyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  if(!CheckDestFolder()){
    if(!MountDestFolder()){
      dbg_printc(LOG_ERR,"RemoteSyncManager","SyncFolder", "Failed to mount destination folder, abort syncronisation");
      return false;
    }
    // Check again just to be shure
    if(!CheckDestFolder()){
      dbg_printc(LOG_ERR,"RemoteSyncManager","SyncFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
      return false;
    }
  }

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

  return true;
  
}
