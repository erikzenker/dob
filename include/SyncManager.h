#ifndef SyncManager_H
#define SyncManager_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

using namespace std;

/*
 * @brief Syncronises a source with destination
 * @class SyncManager
 *        SyncManager.h 
 *        "include/SyncManager.h"
 * @todo  Check destination folder whether it
 *        was correctly mounted
 *
 * The SyncManager knows a destination folder where
 * it should syncronise data from. It gets a 
 * source folder from which it sould syncronise from
 * by using SyncSourceFolder. Several SyncManager
 * can be implemented by using diffent syncronise
 * tools (cp, rsync etc.)
 */
class SyncManager{
protected:
  string mDestFolder;
public:
  SyncManager(string destFolder);
  SyncManager();
  virtual void SyncSourceFolder(string sourceFolder) =0;
  virtual void SyncFolder(string sourceFolder, string syncFolder, string folder) =0;
  virtual void SyncFile(string sourceFolder, string syncFolder) =0;

};

#endif /* SyncManager_H */
