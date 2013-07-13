#include "SshSyncManager.h"

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
SshSyncManager::SshSyncManager( std::string destFolder, 
				      std::string syncType, 
				      std::string destUser, 
				      std::string destHost,
				      std::string destPort):
  SyncManager(destFolder, syncType),
  mDestUser( destUser ),
  mDestHost( destHost )
  {
    if(!destPort.compare(""))
      mDestPort = "22"; // default ssh port
    else
      mDestPort = destPort;

    mAllOptionsPush =  "rsync -vruLKpt --progress --inplace -e \"ssh -l";

    mAllOptionsPush
      .append(mDestUser)
      .append(" -p")
      .append(mDestPort)
      .append("\" ");
    mAllOptionsPull = mAllOptionsPush;
}


bool SshSyncManager::syncSourceFolder(std::string sourceFolder){
  dbg_printc(LOG_DBG, "SshSyncManager","SyncSourceFolder", "Syncronise source and destination folder");
  std::string rsync_push_query = mAllOptionsPush;
  std::string rsync_pull_query = mAllOptionsPull;

  rsync_push_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestUser)
    .append("@")
    .append(mDestHost)
    .append(":")
    .append(mDestFolder);

  rsync_pull_query
    .append(mDestUser)
    .append("@")
    .append(mDestHost)
    .append(":")
    .append(mDestFolder)
    .append(" ")
    .append(sourceFolder);

  dbg_printc(LOG_DBG, "SshSyncManager", "SyncSourceFolder", "Rsync pull string: %s", rsync_pull_query.c_str());
  std::cerr << "\n";
  system(rsync_pull_query.c_str());
  dbg_printc(LOG_DBG, "SshSyncManager", "SyncSourceFolder", "Rsync push string: %s", rsync_push_query.c_str());
  system(rsync_push_query.c_str());

  return true;
}

bool SshSyncManager::syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  std::string rsync_query = mAllOptionsPush; 
  rsync_query
    .append(syncFolder)
    .append(folder)
    .append(" ")
    .append(mDestUser)
    .append("@")
    .append(mDestHost)
    .append(":")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG,"SshSyncManager","SyncFolder","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;
  
}

bool SshSyncManager::syncFile(std::string sourceFolder, std::string syncFolder){
  std::string rsync_query = mAllOptionsPush;
  rsync_query
    .append(syncFolder)
    //.append(folder)
    .append(" ")
    .append(mDestUser)
    .append("@")
    .append(mDestHost)
    .append(":")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG,"SshSyncManager","SyncFile","%s ", rsync_query.c_str());  
  system(rsync_query.c_str());

  return true;

}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool SshSyncManager::removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  
  std::string rm_query = mAllOptionsPush; 
  rm_query
    .append("--delete ")
    .append(syncFolder)
    .append(" ")
    .append(mDestUser)
    .append("@")
    .append(mDestHost)
    .append(":")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  dbg_printc(LOG_DBG, "SshSyncManager","RemoveFolder","%s", rm_query.c_str());
  system(rm_query.c_str());

  return true;

}

/**
 * @todo fill with content
 *
 **/
bool SshSyncManager::checkDestination(){
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool SshSyncManager::setupDestination(){
  return true;
}
