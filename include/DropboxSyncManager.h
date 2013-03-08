/** 
 * @file      DropboxSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef DropboxSyncManager_H
#define DropboxSyncManager_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <SyncManager.h>
#include <dbg_print.h>

/**
 * @brief Syncronises a source with destination
 * @class DropboxSyncManager
 *        DropboxSyncManager.h 
 *        "include/DropboxSyncManager.h"
 *
 **/
class DropboxSyncManager : public SyncManager{
public:
  DropboxSyncManager(std::string destFolder, string syncType);
  ~DropboxSyncManager();
  virtual bool syncSourceFolder(std::string sourceFolder);
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder);
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder);

protected:
  virtual bool checkDestination();
  virtual bool setupDestination();

  std::string mOauthKey;
  std::string mOauthSecret;
  std::string mToken;
  std::string mSecretToken;
  std::string mAccesToken;
  std::string mSecretAccesToken;
  std::string mRequestUrl;

};

#endif /* DropboxSyncManager_H */
