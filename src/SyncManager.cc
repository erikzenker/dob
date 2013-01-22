#include "SyncManager.h"


SyncManager::SyncManager(string destFolder, string syncType) : 
  mDestFolder(destFolder),
  mSyncType(syncType){
  
  if(mDestFolder[mDestFolder.size()-1] != '/')
     mDestFolder.append("/");

}

SyncManager::SyncManager(){

}

/*
 * @note  Rsync command line parameters
 *        -r, --recursive         recurse into directories
 *        -u, --update            skip files that are newer on the receiver
 *        -z, --compress          compress file data during the transfer
 *        -v, --verbose           increase verbosity
 *            --delete            delete extraneous files from dest dirs
 *        -L, --copy-links        transform symlink into referent file/dir
 *        -K, --keep-dirlinks     treat symlinked dir on receiver as dir
 *        -p, --perms             preserve permissions
 *        -t, --times             preserve modification times
 *        -o, --owner             preserve owner (super-user only)
 *        -g, --group             preserve group
 *            --devices           preserve device files (super-user only)
 *            --specials          preserve special files
 *        -D                      same as --devices --specials
 *            --modify-window=NUM compare mod-times with reduced accuracy
 *        See more in "man rsync"
 */
bool SyncManager::SyncSourceFolder(string sourceFolder){
  if(!CheckDestFolder()){
    if(!MountDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","SyncSourceFolder", "Failed to mount destination folder, abort syncronisation");
      return false;
    }
    // Check again just to be shure
    if(!CheckDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","SyncSourceFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
      return false;
    }
  }


  dbg_printc(LOG_DBG, "SyncManager", "SyncSourceFolder", "Syncronise source and destination folder");
  string push_query = "rsync -vzruLKpt --modify-window=1 ";
  string pull_query = "rsync -vzruLKpt --modify-window=1 ";
  push_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestFolder);
  pull_query
    .append(mDestFolder)      
    .append(" ")
    .append(sourceFolder);
  cerr << "\n";
  system(pull_query.c_str());
  system(push_query.c_str());

  return true;
}

bool SyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  if(!CheckDestFolder()){
    if(!MountDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","SyncFolder", "Failed to dispatch destination folder, abort syncronisation");
      return false;
    }
    // Check again just to be shure
    if(!CheckDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","SyncFolder", "Failed syncronise source and destination folder, because destination folder is not reachable or not mounted");
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
    dbg_printc(LOG_DBG, "SyncManager","SyncFolder","Can't reach destination folder, maybe location is offline or file doesn't exist anymore");
    return false;
  }
  dbg_printc(LOG_DBG,"SyncManager","SyncFolder","%s ", cp_query.c_str());

  return true;
  
}

bool SyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  if(!CheckDestFolder()){
    if(!MountDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","RemoveFolder", "Failed to mount destination folder, abort syncronisation");
      return false;
    }
    // Check again just to be shure
    if(!CheckDestFolder()){
      dbg_printc(LOG_ERR,"SyncManager","RemoveFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
      return false;
    }
  }
  string rm_query = "rsync -vruLKpt --delete --progress --inplace ";
  rm_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG, "RemoteSyncManager","RemoveFolder","%s", rm_query.c_str());
  system(rm_query.c_str());

  return true;
}

bool SyncManager::SyncFile(string sourceFolder, string syncFolder){
  string rsync_query = "rsync -vruLKpt --progress --inplace ";  
  rsync_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG,"RemoteSyncManager","SyncFile","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;

}

