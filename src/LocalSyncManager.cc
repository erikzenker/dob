#include <LocalSyncManager.h>
#include <FileStateDatabase.h>
#include <boost/filesystem.hpp>
#include <dbg_print.h>

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
LocalSyncManager::LocalSyncManager(boost::filesystem::path destFolder, SyncType syncType):
  SyncManager(destFolder, syncType),
  mRsyncOptions("rsync -qvruLKpt --progress --inplace "){

}

bool LocalSyncManager::syncInitial(boost::filesystem::path rootPath){
  dbg_printc(LOG_DBG, "LocalManager", "SyncSourceFolder", "Syncronise source and destination folder");
  std::string push_query = mRsyncOptions + rootPath.string() + " " + mDestPath.string();
  std::string pull_query = mRsyncOptions + mDestPath.string()  + " " + rootPath.string();

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

bool LocalSyncManager::syncFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string relativePath = fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  std::string cp_query = "cp -RLv " + fullPath.string() + " " + mDestPath.string() +  relativePath + " ";  
  
  if(system(cp_query.c_str())){
    dbg_printc(LOG_DBG, "LocalManager","SyncFolder","Can't reach destination folder, maybe location is offline or file doesn't exist anymore");
    return false;
  }
  dbg_printc(LOG_DBG,"LocalManager","SyncFolder","%s ", cp_query.c_str());

  return true;
  
}

bool LocalSyncManager::removeFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string relativePath = fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  std::string rm_query = mRsyncOptions + "--delete " + fullPath.string() + " " + mDestPath.string() + relativePath;

  dbg_printc(LOG_DBG, "LocalSyncManager","RemoveFolder","%s", rm_query.c_str());
  system(rm_query.c_str());

  return true;
}

bool LocalSyncManager::syncFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms){
  std::string relativePath = fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  std::string rsync_query = mRsyncOptions + fullPath.string() + " " + mDestPath.string() + relativePath;

  dbg_printc(LOG_DBG,"LocalSyncManager","SyncFile","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;

}
