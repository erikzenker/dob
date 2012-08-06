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

  if(system(mountpoint_query.c_str())){
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
