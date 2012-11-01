#ifndef RemoteSyncManager_H
#define RemoteSyncManager_H

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

class RemoteSyncManager : public SyncManager{

 public:
  RemoteSyncManager(string destFolder);
  virtual bool SyncSourceFolder(string sourceFolder);
  virtual bool SyncFolder(string sourceFolder, string syncFolder, string folder);
  virtual bool SyncFile(string sourceFolder, string syncFolder);
  virtual bool RemoveFolder(string sourceFolder, string syncFolder, string folder);

 protected:
  bool isMountpoint(string mountpoint);

};

#endif /* RemoteSyncManager_H */
