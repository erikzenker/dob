/** 
 * @file      SyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef SyncManager_H
#define SyncManager_H

#include <gtkmm.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <dbg_print.h>

using namespace std;

/**
 * @brief Syncronises a source with destination
 * @class SyncManager
 *        SyncManager.h 
 *        "include/SyncManager.h"
 *
 * The SyncManager knows a destination folder where
 * it should syncronise data to. It gets a 
 * source folder from which it sould syncronise from
 * Several SyncManager can be implemented by using 
 * diffent syncronise tools (cp, rsync, git etc.)
 **/
class SyncManager{
public:
  SyncManager(string destFolder, string syncType);
  ~SyncManager();
  virtual bool syncSourceFolder(string sourceFolder) =0;
  virtual bool syncFolder(string sourceFolder, string syncFolder, string folder) =0;
  virtual bool syncFile(string sourceFolder, string syncFolder) =0;
  virtual bool removeFolder(string sourceFolder, string syncFolder, string folder) =0;

protected:
  virtual bool checkDestFolder() =0;
  virtual bool mountDestFolder() =0;
  string mDestFolder;
  string mSyncType;

};

#endif /* SyncManager_H */
