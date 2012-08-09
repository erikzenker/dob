#ifndef RemoteSyncManager_H
#define RemoteSyncManager_H

#include <string>
#include "SyncManager.h"

using namespace std;

class RemoteSyncManager : public SyncManager{

 public:
  RemoteSyncManager(string destFolder);
  RemoteSyncManager();
  void SyncSourceFolder(string sourceFolder);
  void SyncFolder(string sourceFolder, string syncFolder, string folder);
  void SyncFile(string sourceFolder, string syncFolder);


};

#endif /* RemoteSyncManager_H */
