#ifndef SyncManager_H
#define SyncManager_H

#include <gtkmm.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

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
 * by using SyncSourceFolder. Several SyncManager
 * can be implemented by using diffent syncronise
 * tools (cp, rsync etc.)
 **/
class SyncManager{
public:
  SyncManager(string destFolder, string syncType);
  SyncManager();
  virtual bool SyncSourceFolder(string sourceFolder) =0;
  virtual bool SyncFolder(string sourceFolder, string syncFolder, string folder) =0;
  virtual bool SyncFile(string sourceFolder, string syncFolder) =0;
  virtual bool RemoveFolder(string sourceFolder, string syncFolder, string folder) =0;

protected:
  string mDestFolder;
  string mSyncType;
};

#endif /* SyncManager_H */
