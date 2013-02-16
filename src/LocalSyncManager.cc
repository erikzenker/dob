#include "LocalSyncManager.h"

LocalSyncManager::LocalSyncManager(std::string destFolder, std::string syncType):
  SyncManager(destFolder, syncType){

}

bool LocalSyncManager::checkDestination(){
  static struct stat64 my_stat;
  if ( -1 == lstat64(mDestFolder.c_str(), &my_stat ) ) {
    if (errno == ENOENT) return false;
    dbg_printc(LOG_WARN, "LocalSyncManager", "IsDir", "\nC Stat failed on %s: %d\n", mDestFolder.c_str(), errno);
    return false;
  }
  if(S_ISDIR( my_stat.st_mode ))
    return true;
  return false;
}

bool LocalSyncManager::setupDestination(){
  return checkDestination();
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
bool LocalSyncManager::syncSourceFolder(std::string sourceFolder){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"SyncManager","SyncSourceFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
    return false;
  }


  dbg_printc(LOG_DBG, "SyncManager", "SyncSourceFolder", "Syncronise source and destination folder");
  std::string push_query = "rsync -vzruLKpt --modify-window=1 ";
  std::string pull_query = "rsync -vzruLKpt --modify-window=1 ";
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

bool LocalSyncManager::syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"SyncManager","SyncFolder", "Failed syncronise source and destination folder, because destination folder is not reachable or not mounted");
    return false;
  }

  std::string cp_query = "cp -RLv ";  
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

bool LocalSyncManager::removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"SyncManager","RemoveFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
    return false;
  }

  std::string rm_query = "rsync -vruLKpt --delete --progress --inplace ";
  rm_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG, "RemoteSyncManager","RemoveFolder","%s", rm_query.c_str());
  system(rm_query.c_str());

  return true;
}

bool LocalSyncManager::syncFile(std::string sourceFolder, std::string syncFolder){
  std::string rsync_query = "rsync -vruLKpt --progress --inplace ";  
  rsync_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG,"RemoteSyncManager","SyncFile","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;

}
