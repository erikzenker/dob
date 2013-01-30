#include "GitSyncManager.h"

GitSyncManager::GitSyncManager(std::string destFolder, std::string syncType, std::string destProtocol):
  SyncManager(destFolder, syncType),
  mDestProtocol(destProtocol),
  mGitIsInitialized(false){

  std::string git_init_query = "git init ";
  git_init_query
    .append(mDestFolder)
    .append(" --bare");
  system(git_init_query.c_str());

}

/**
 *  @todo check destFolder, sourcefolder for "\" as last char
 *
 **/
bool GitSyncManager::syncSourceFolder(std::string sourceFolder){
  dbg_printc(LOG_DBG, "GitSyncManager","SyncSourceFolder", "Syncronise source and destination folder");

  std::string git_add_query    = "cd ";
  std::string git_commit_query = "cd "; 
  std::string git_pull_query   = "cd "; 
  std::string git_push_query   = "cd "; 
  std::string git_init_query = "git init ";

  git_init_query
    .append(sourceFolder);

  if(!mGitIsInitialized){
    system(git_init_query.c_str());
    dbg_printc(LOG_DBG, "GitSyncManager", "SyncSourceFolder", "git init std::string: %s", git_init_query.c_str());
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

bool GitSyncManager::syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  return syncSourceFolder(sourceFolder);
  
}

bool GitSyncManager::syncFile(std::string sourceFolder, std::string syncFolder){
   return syncSourceFolder(sourceFolder);
  
}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool GitSyncManager::removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  return syncSourceFolder(sourceFolder);

}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::checkDestFolder(){
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool GitSyncManager::mountDestFolder(){
  return false;
}
