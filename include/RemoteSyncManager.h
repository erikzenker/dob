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
 * local and remote data. Rsync should
 * run in deamon mode on the server.
 *
 **/
class RemoteSyncManager : public SyncManager{
public:
  RemoteSyncManager(  std::string destFolder, 
		      std::string syncType, 
		      std::string destUser, 
		      std::string destHost, 
		      std::string destPort, 
		      std::string sshPort );
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
  std::string mSshPort;
  std::string mAllOptionsPush;
  std::string mAllOptionsPull;
  

};

#endif /* RemoteSyncManager_H */
