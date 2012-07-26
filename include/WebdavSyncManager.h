#ifndef WebdavSyncManager_H
#define WebdavSyncManager_H

#include <string>
#include "RemoteSyncManager.h"

using namespace std;

class WebdavSyncManager : public RemoteSyncManager{

 public:
  WebdavSyncManager(string destFolder);

};

#endif /* WebdavSyncManager_H */
