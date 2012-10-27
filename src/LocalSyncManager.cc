#include "LocalSyncManager.h"

LocalSyncManager::LocalSyncManager(string destFolder):
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
bool LocalSyncManager::SyncSourceFolder(string sourceFolder){
  if(IsDir(mDestFolder)){
    cerr << "\nC Syncronise source and destination folder\n";
    string rsync_query = "rsync -vzruL --delete ";
    rsync_query
      .append(sourceFolder)
      .append(" ")
      .append(mDestFolder);
  
    system(rsync_query.c_str());
  }
  else{
    cerr << "\nC Failed syncronise source and destination folder, because destination folder is not directory";

  }
  return true;
  
}

bool LocalSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  if(IsDir(mDestFolder)){
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
    cerr << "\nC Failed syncronise source and destination folder, because destination folder is not a directory";

  }
  return true;
  
}

bool LocalSyncManager::SyncFile(string sourceFolder, string syncFolder){
  return true;

}

bool LocalSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  
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

bool LocalSyncManager::IsDir(string directory){

  static struct stat64 my_stat;

  if ( -1 == lstat64(directory.c_str(), &my_stat ) ) {
    if (errno == ENOENT) return false;
    fprintf(stderr, "Stat failed on %s: %s\n", directory.c_str(), strerror(errno));
    return false;
  }
  if(S_ISDIR( my_stat.st_mode ))
    return true;
  return false;

}
