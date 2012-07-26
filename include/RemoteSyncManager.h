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

};

#endif /* RemoteSyncManager_H */
