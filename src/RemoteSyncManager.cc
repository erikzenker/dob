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
  cerr << "\nC Syncronise Source and Destination Folder\n";
  string rsync_query = "rsync -vzruL --delete ";
  rsync_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestFolder);
  
  system(rsync_query.c_str());


}
