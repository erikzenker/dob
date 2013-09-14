#include <LocalSyncManager.h>
#include <boost/filesystem.hpp>

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
 *        -q  --quiet             suppress non-error messages
 *        See more in "man rsync"
 */
LocalSyncManager::LocalSyncManager(std::string destFolder, SyncType syncType):
  SyncManager(destFolder, syncType),
  mRsyncOptions("rsync -qvruLKpt --progress --inplace "){

}

bool LocalSyncManager::checkDestination(){
  boost::filesystem::path destPath(mDestFolder);
  if(boost::filesystem::is_directory(destPath))
    return true;
  return false;
  
}

bool LocalSyncManager::setupDestination(){
  return checkDestination();
}

bool LocalSyncManager::syncSourceFolder(std::string rootPath){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"LocalManager","SyncSourceFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
    return false;
  }

  dbg_printc(LOG_DBG, "LocalManager", "SyncSourceFolder", "Syncronise source and destination folder");
  std::string push_query = mRsyncOptions + rootPath + " " + mDestFolder;
  std::string pull_query = mRsyncOptions + mDestFolder  + " " + rootPath;

  switch(mSyncType){
  case DOB_BACKUP:
    system(push_query.c_str());
    break;
  case DOB_UPDATE:
    system(pull_query.c_str());
    break;
  case DOB_SYNCRONIZE:
    system(pull_query.c_str());
    system(push_query.c_str());
  default:
    break;
  };

  return true;
}

bool LocalSyncManager::syncFolder(std::string rootPath, std::string syncFolder, std::string folder){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"LocalManager","SyncFolder", "Failed syncronise source and destination folder, because destination folder is not reachable or not mounted");
    return false;
  }

  std::string relativePath = syncFolder.substr(rootPath.length(), syncFolder.length());
  std::string cp_query = "cp -RLv " + syncFolder + folder + " " + mDestFolder +  relativePath + " ";  
  
  if(system(cp_query.c_str())){
    dbg_printc(LOG_DBG, "LocalManager","SyncFolder","Can't reach destination folder, maybe location is offline or file doesn't exist anymore");
    return false;
  }
  dbg_printc(LOG_DBG,"LocalManager","SyncFolder","%s ", cp_query.c_str());

  return true;
  
}

bool LocalSyncManager::removeFolder(std::string rootPath, std::string syncFolder, std::string folder){
  if(!checkDestination()){
    dbg_printc(LOG_ERR,"LOcalManager","RemoveFolder", "Failed syncronise source and destination folder, because destination folder is not mounted");
    return false;
  }

  std::string relativePath = syncFolder.substr(rootPath.length(), syncFolder.length());
  std::string rm_query = mRsyncOptions + "--delete " + syncFolder + " " + mDestFolder + relativePath;

  dbg_printc(LOG_DBG, "LocalSyncManager","RemoveFolder","%s", rm_query.c_str());
  system(rm_query.c_str());

  return true;
}

bool LocalSyncManager::syncFile(std::string rootPath, std::string syncFolder){
  std::string relativePath = syncFolder.substr(rootPath.length(), syncFolder.length());
  std::string rsync_query = mRsyncOptions + syncFolder + " " + mDestFolder + relativePath;

  dbg_printc(LOG_DBG,"LocalSyncManager","SyncFile","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;

}
