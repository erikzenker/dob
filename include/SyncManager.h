/** 
 * @file      SyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef SyncManager_H
#define SyncManager_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <dbg_print.h>
#include <SyncType.h>

/**
 * @brief Syncronises a source with destination
 * @class SyncManager
 *        SyncManager.h 
 *        "include/SyncManager.h"
 *
 * The SyncManager knows a destination where
 * it should syncronise data to. It gets a 
 * source folder from which it sould syncronise from
 * Several SyncManager can be implemented by using 
 * diffent syncronise tools (cp, rsync, git etc.)
 **/
class SyncManager{
public:
  SyncManager(std::string destFolder, SyncType syncType);
  ~SyncManager();
  virtual bool syncSourceFolder(std::string sourceFolder) =0;
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder) =0;
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder) =0;
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder) =0;

protected:
  virtual bool checkDestination() =0;
  virtual bool setupDestination() =0;
  std::string mDestFolder;
  SyncType mSyncType;

};

#endif /* SyncManager_H */
