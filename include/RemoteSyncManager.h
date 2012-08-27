#ifndef RemoteSyncManager_H
#define RemoteSyncManager_H

#include <string>
#include "SyncManager.h"

using namespace std;

/**
 *
 * @todo SyncFolder: if file does not exist anymore no cp fail should be raised
 *
 **/
class RemoteSyncManager : public SyncManager{

 public:
  RemoteSyncManager(string destFolder);
  RemoteSyncManager();
  bool SyncSourceFolder(string sourceFolder);
  bool SyncFolder(string sourceFolder, string syncFolder, string folder);
  bool SyncFile(string sourceFolder, string syncFolder);
  bool RemoveFolder(string sourceFolder, string syncFolder, string folder);

};

#endif /* RemoteSyncManager_H */
