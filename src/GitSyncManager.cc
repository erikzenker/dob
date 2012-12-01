#include "GitSyncManager.h"

GitSyncManager::GitSyncManager(string destFolder, string syncType, string destProtocol):
  SyncManager(destFolder, syncType),
  mDestProtocol(destProtocol){

}

/**
 *  @todo check destFolder, sourcefolder for "\" as last char
 *
 **/
bool GitSyncManager::SyncSourceFolder(string sourceFolder){
  dbg_printc(LOG_DBG, "GitSyncManager","SyncSourceFolder", "Syncronise source and destination folder");

  string git_add_query    = "cd ";
  string git_commit_query = "cd "; 
  string git_pull_query   = "cd "; 
  string git_push_query   = "cd "; 

  git_add_query
    .append(sourceFolder)
    .append(" && ")
    .append("git add *");

  git_commit_query
    .append(sourceFolder)
    .append(" && ")
    .append("git commit -a -m \"test-commit\" ");

  git_pull_query
    .append(sourceFolder)
    .append(" && ")
    .append("git pull ")
    .append(mDestFolder);

  git_push_query
    .append(sourceFolder)
    .append(" && ")
    .append("git push ")
    .append(mDestFolder);

  cerr << "\n";
  dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git add string: %s", git_add_query.c_str());
  dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git commit string: %s", git_commit_query.c_str());
  dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git pull string: %s", git_pull_query.c_str());
  dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git push string: %s", git_push_query.c_str());
  system(git_add_query.c_str());
  system(git_commit_query.c_str());
  system(git_pull_query.c_str());
  system(git_push_query.c_str());

  return true;
}

bool GitSyncManager::SyncFolder(string sourceFolder, string syncFolder, string folder){
  /*
  string cp_query = "rsync -vzruLKpt ";  
  cp_query
    .append(syncFolder)
    .append(folder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));
  
  if(system(cp_query.c_str())){
    dbg_printc(LOG_ERR, "GitSyncManager","SyncFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG,"GitSyncManager","SyncFolder","%s ", cp_query.c_str());

  return true;
  */
  return SyncSourceFolder(sourceFolder);
  
}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool GitSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  /*
  string rm_query = "rsync -vzruLKpt --delete ";
  rm_query
    .append(syncFolder)
    .append(" ")
    .append(mDestFolder)
    .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  if(system(rm_query.c_str())){
    dbg_printc(LOG_ERR, "GitSyncManager","RemoveFolder","Can't reach destination folder, maybe location is offline");
    return false;
  }
  dbg_printc(LOG_DBG, "GitSyncManager","RemoveFolder","%s", rm_query.c_str());
  return true;
  */
  return SyncSourceFolder(sourceFolder);

}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::CheckDestFolder(){
  /*
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);
  return !system(mountpoint_query.c_str());
  */
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::MountDestFolder(){
  /*
  if(!mMountOptions.compare(""))
    return false;

  string mount_query;
  mount_query
    .append("sshfs -o ServerAliveInterval=15 ")
    .append(mMountOptions)
    .append(" ")
    .append(mDestFolder);
  cerr << mount_query << "\n";
  system(mount_query.c_str());

  return true;
  */
  return false;
}
