/* 
 * @file      GitSyncManager.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 */
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

using namespace std;

class GitSyncManager : public SyncManager{

 public:
  GitSyncManager(string destFolder, string syncType, string destProtocol);
  virtual bool SyncSourceFolder(string sourceFolder);
  virtual bool SyncFolder(string sourceFolder, string syncFolder, string folder);
  virtual bool RemoveFolder(string sourceFolder, string syncFolder, string folder);
  virtual bool SyncFile(string sourceFolder, string syncFolder);

 protected:
  virtual bool CheckDestFolder();
  virtual bool MountDestFolder();

 private:
  string mDestProtocol;
  bool mGitIsInitialized;
};

#endif /* GitSyncManager_H */
