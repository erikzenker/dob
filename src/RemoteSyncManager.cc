#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder){
  mDestFolder = destFolder;

}

RemoteSyncManager::RemoteSyncManager(){

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
void RemoteSyncManager::SyncSourceFolder(string sourceFolder){
  // Mountpoint checks whether the remote device is mounted.
  // Exit status zero if the directory is a mountpoint, 
  // non-zero if not.
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);

  if(!system(mountpoint_query.c_str())){
    cerr << "\nC Syncronise source and destination folder\n";
    string rsync_query = "rsync -vzruL --delete ";
    rsync_query
      .append(sourceFolder)
      .append(" ")
      .append(mDestFolder);
  
    system(rsync_query.c_str());
  }
  else{
    cerr << "\nC Failed syncronise source and destination folder, because destination folder is not mounted";

  }
  
}

void RemoteSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
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

  string cp_query = "cp -RLv ";  
  cp_query
    .append(syncFolder)
    .append(folder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));
  
  cerr << "\nC " << cp_query;
  system(cp_query.c_str());
  


}

void RemoteSyncManager::SyncFile(string sourceFolder, string syncFolder){


}

void RemoteSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){

  string rm_query = "rm -Rv ";
  rm_query
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()))
    .append(folder);

  cerr << "\nC " << rm_query << "\n";
  system(rm_query.c_str());


}
