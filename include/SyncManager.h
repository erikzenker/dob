/** 
 * @file      SyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef SyncManager_H
#define SyncManager_H

#include <string>
#include <SyncType.h>
#include <FileStateDatabase.h>
#include <boost/filesystem.hpp>

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
  SyncManager(boost::filesystem::path destPath, SyncType syncType);
  ~SyncManager();

  virtual bool pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) =0;
  virtual bool pushDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) =0;
  virtual bool removeFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) =0;
  virtual bool removeDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) =0;

protected:
  boost::filesystem::path mDestPath;
  SyncType mSyncType;

};

#endif /* SyncManager_H */
