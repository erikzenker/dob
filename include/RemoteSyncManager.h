/** 
 * @file      RemoteSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

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

/**
 * @brief Syncronization with remote host.
 * @class RemoteSyncManager
 *        RemoteSyncManager.h
 *        "include/RemoteSyncManager.h"
 * 
 * The tool rsync is used to syncronize
 * local and remote data. There is no
 * versioning inside, so data could be
 * doubled and appear again.
 *
 **/
class RemoteSyncManager : public SyncManager{
public:
  RemoteSyncManager(std::string destFolder, std::string syncType, std::string destProtocol);
  virtual bool syncSourceFolder(std::string sourceFolder);
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder);
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder);

protected:
  virtual bool checkDestination();
  virtual bool setupDestination();

private:
  std::string mDestProtocol;
  

};

#endif /* RemoteSyncManager_H */
