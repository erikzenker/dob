/** 
 * @file      SshSyncManager.h
 * @author    Erik Zenker
 * @date      18.02.2013
 * @copyright Gnu Public License
 */

#ifndef SshSyncManager_H
#define SshSyncManager_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
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
 * local and remote data over ssh. There is 
 *  no versioning inside, so data could be
 * doubled and appear again.
 *
 **/
class SshSyncManager : public SyncManager{
public:
  SshSyncManager(  std::string destFolder, 
		      std::string syncType, 
		      std::string destUser, 
		      std::string destHost, 
		      std::string destPort);
  virtual bool syncSourceFolder(std::string sourceFolder);
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder);
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder);

protected:
  virtual bool checkDestination();
  virtual bool setupDestination();

private:
  std::string mDestPort;
  std::string mDestUser;
  std::string mDestHost;
  std::string mAllOptionsPush;
  std::string mAllOptionsPull;
  

};

#endif /* SshSyncManager_H */
