/** 
 * @file      SyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#pragma once
#include <string>
#include <boost/filesystem.hpp>

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
  SyncManager(boost::filesystem::path destPath, SyncType syncType);
  ~SyncManager();

  virtual bool pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const =0;
  virtual bool pushDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const =0;
  virtual bool mkdir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const =0;
  virtual bool removeFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const =0;
  virtual bool removeDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath) const =0;

protected:
  const boost::filesystem::path destPath;
  const SyncType syncType;

};


