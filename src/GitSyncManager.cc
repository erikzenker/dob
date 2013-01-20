#include "GitSyncManager.h"

GitSyncManager::GitSyncManager(string destFolder, string syncType, string destProtocol):
  SyncManager(destFolder, syncType),
  mDestProtocol(destProtocol),
  mGitIsInitialized(false){

  string git_init_query = "git init ";
  git_init_query
    .append(mDestFolder)
    .append(" --bare");
  system(git_init_query.c_str());

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
  string git_init_query = "git init ";

  git_init_query
    .append(sourceFolder);

  if(!mGitIsInitialized){
    system(git_init_query.c_str());
    dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git init string: %s", git_init_query.c_str());
    mGitIsInitialized = true;
  }

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
  return SyncSourceFolder(sourceFolder);
  
}

bool GitSyncManager::SyncFile(string sourceFolder, string syncFolder){
   return SyncSourceFolder(sourceFolder);
  
}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool GitSyncManager::RemoveFolder(string sourceFolder, string syncFolder, string folder){
  return SyncSourceFolder(sourceFolder);

}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::CheckDestFolder(){
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::MountDestFolder(){
  return false;
}
