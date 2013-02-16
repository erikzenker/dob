/**
 * @file      GitSyncManager.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 **/

#ifndef GitSyncManager_H
#define GitSyncManager_H

#include <string>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <SyncManager.h>
#include <dbg_print.h>

/**
 * @brief Syncronization with git repository
 * @class GitSyncManager
 *        GitSyncManager.h
 *        "include/GitSyncManager.h"
 * 
 * The try to have versioning on syncronization,
 * BUT git ignores symlinks. Finally it does not
 * fit into the symlink schema.
 **/
class GitSyncManager : public SyncManager{

 public:
  GitSyncManager(std::string destFolder, std::string syncType, std::string destProtocol);
  virtual bool syncSourceFolder(std::string sourceFolder);
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder);

 protected:
  virtual bool checkDestination();
  virtual bool setupDestination();

 private:
  std::string mDestProtocol;
  bool mGitIsInitialized;
};

#endif /* GitSyncManager_H */
