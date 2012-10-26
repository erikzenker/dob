#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder):
  SyncManager(destFolder){

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
    cerr << "\nC Syncronise source and destination folder\n";
    string rsync_query = "rsync -vzruL --delete ";
    rsync_query
      .append(sourceFolder)
      .append(" ")
      .append(mDestFolder)
      .append(" &");
    system(rsync_query.c_str());

  }
  else{
    cerr << "\nC Failed syncronise source and destination folder, because destination folder is not mounted";

  }
  return true;
  
}

bool RemoteSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  /* RSYNC MOD
  string rsync_query = "rsync -vzruL --delete ";
  rsync_query
    .append(syncFolder)
    .append(folder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));
  cerr << "\nC " << rsync_query;
  system(rsync_query.c_str());
  */
  if(isMountpoint(mDestFolder)){
    string cp_query = "cp -RLv ";  
    cp_query
      .append(syncFolder)
      .append(folder)
      .append(" ")
      .append(mDestFolder)
      .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
      .append(" ");
  
    cerr << "\nC " << cp_query;
    if(system(cp_query.c_str())){
      cerr << "\nC Can't reach destination folder, maybe location is offline";
      return false;
    }
  }
  else{
    cerr << "\nC Failed syncronise source and destination folder, because destination folder is not mounted";

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

  cerr << "\nC " << rm_query << "\n";
  if(system(rm_query.c_str())){
    cerr << "\nC Can't reach destination folder, maybe location is offline";
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
