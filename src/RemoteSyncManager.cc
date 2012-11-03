#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder, string syncType):
  SyncManager(destFolder, syncType){

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
bool RemoteSyncManager::SyncSourceFolder(string sourceFolder){
  if(isMountpoint(mDestFolder)){
    dbg_print(LOG_DBG, "\nC RemoteSyncManager::SyncSourceFolder: Syncronise source and destination folder");
    string rsync_query = "rsync -vzruL --delete ";
    rsync_query
      .append(sourceFolder)
      .append(" ")
      .append(mDestFolder)
      .append(" &");
    cerr << "\n";
    system(rsync_query.c_str());

  }
  else{
    dbg_print(LOG_DBG, "\nC RemoteSyncManager::SyncSourceFolder: Failed syncronise source and destination folder, because destination folder is not mounted");

  }
  return true;
  
}

bool RemoteSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  if(isMountpoint(mDestFolder)){
    string cp_query = "cp -RLv ";  
    cp_query
      .append(syncFolder)
      .append(folder)
      .append(" ")
      .append(mDestFolder)
      .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
      .append(" ");
  
    dbg_print(LOG_DBG,"\nC RemoteSyncManager::SyncFolder: %s ", cp_query.c_str());
    if(system(cp_query.c_str())){
      dbg_print(LOG_DBG, "\nC RemoteSyncManager::SyncFolder: Can't reach destination folder, maybe location is offline");
      return false;
    }
  }
  else{
    dbg_print(LOG_DBG, "\nC RemoteSyncManager::SyncFolder: Failed syncronise source and destination folder, because destination folder is not mounted");

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

  dbg_print(LOG_DBG, "\nC RemoteSyncManager:RemoveFolder: %s\n", rm_query.c_str());
  if(system(rm_query.c_str())){
    dbg_print(LOG_DBG, "\nC RemoteSyncManager:RemoveFolder: Can't reach destination folder, maybe location is offline");
    return false;
  }
  return true;

}

/**
 * Mountpoint checks whether the remote device is mounted.
 * Exit status zero if the directory is a mountpoint, 
 * non-zero if not.
 *
 *
 **/
bool RemoteSyncManager::isMountpoint(string mountpoint){
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mountpoint);
  return !system(mountpoint_query.c_str());
 
}
