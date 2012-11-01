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
    dbg_print(LOG_DBG, "\nC LocalSyncManager::SyncSourceFolder: Syncronise source and destination folder");

    string rsync_query = "rsync -vzruL --delete ";
    rsync_query
      .append(sourceFolder)
      .append(" ")
      .append(mDestFolder);
    cerr << "\n";
    system(rsync_query.c_str());
  }
  else{
    dbg_print(LOG_DBG, "\nC LocalSyncManager::SyncSourceFolder: Failed syncronise source and destination folder, because destination folder is not mounted");

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
  
    
    dbg_print(LOG_DBG,"\nC LocalSyncManager::SyncFolder: %s ", cp_query.c_str());
    if(system(cp_query.c_str())){
      dbg_print(LOG_DBG, "\nC LocalSyncManager::SyncFolder: Can't reach destination folder, maybe location is offline");
      return false;
    }
  }
  else{
    dbg_print(LOG_DBG, "\nC LocalSyncManager::SyncFolder: Failed syncronise source and destination folder, because destination folder is not a directory");

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

  dbg_print(LOG_DBG, "\nC LocalSyncManager:RemoveFolder: %s\n", rm_query.c_str());
  if(system(rm_query.c_str())){
    dbg_print(LOG_DBG, "\nC LocalSyncManager:RemoveFolder: Can't reach destination folder, maybe location is offline");
    return false;
    }
  return true;

}

bool LocalSyncManager::IsDir(string directory){

  static struct stat64 my_stat;

  if ( -1 == lstat64(directory.c_str(), &my_stat ) ) {
    if (errno == ENOENT) return false;
    dbg_print(LOG_WARN, "\nC Stat failed on %s: %s\n", directory.c_str(), strerror(errno));
    return false;
  }
  if(S_ISDIR( my_stat.st_mode ))
    return true;
  return false;

}
