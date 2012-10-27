#ifndef LocalSyncManager_H
#define LocalSyncManager_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "SyncManager.h"


class LocalSyncManager : public SyncManager {

public:
  LocalSyncManager(string destFolder);
  virtual bool SyncSourceFolder(string sourceFolder);
  virtual bool SyncFolder(string sourceFolder, string syncFolder, string folder);
  virtual bool SyncFile(string sourceFolder, string syncFolder);
  virtual bool RemoveFolder(string sourceFolder, string syncFolder, string folder);

 protected: 
  bool IsDir(string directory);

};


#endif /* LocalSyncManager_H */
