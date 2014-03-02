/** 
 * @file      LocalSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#pragma once
#include <string>
#include <boost/filesystem.hpp>

#include <SyncType.h>
#include <SyncManager.h>


/**
 * @brief Syncronises a source with destination
 * @class LocalSyncManager
 *        LocalSyncManager.h 
 *        "include/LocalSyncManager.h"
 *
 **/
class LocalSyncManager : public SyncManager{
public:
  LocalSyncManager(boost::filesystem::path destPath, SyncType syncType);

  virtual bool pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const;
  virtual bool pushDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const;
  virtual bool mkdir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const;
  virtual bool removeFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const;
  virtual bool removeDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const;


};


