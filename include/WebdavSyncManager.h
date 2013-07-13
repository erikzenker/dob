/** 
 * @file      WebdavSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef WebdavSyncManager_H
#define WebdavSyncManager_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h> /* lstat */
#include <stdlib.h>
#include <boost/filesystem.hpp> /* filesystem, is_directory, file_size, last_write_time */
#include <map> /* map */

#include <SyncManager.h>
#include <dbg_print.h>
#include <WebdavClient.h>

/**
 * @brief Syncronization with remote host.
 * @class WebdavSyncManager
 *        WebdavSyncManager.h
 *        "include/WebdavSyncManager.h"
 * 
 **/

class WebdavSyncManager : public SyncManager{
public:
  WebdavSyncManager(std::string destFolder, std::string syncType,  std::string destUser, std::string destHost, std::string destPort);
  virtual bool syncSourceFolder(std::string rootPath);
  virtual bool syncFolder(std::string rootPath, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string rootPath, std::string fullPath);
  virtual bool removeFolder(std::string rootPath, std::string syncFolder, std::string folder);

protected:
  virtual bool checkDestination();
  virtual bool setupDestination();

private:
  bool pushFile(std::string rootPath, boost::filesystem::path fullPath);
  bool pushFolder(std::string rootPath, boost::filesystem::path fullPath);
  bool removeFolder(std::string rootPath, boost::filesystem::path fullPath);
  bool pushFolderRecursively(std::string rootPath, boost::filesystem::path fullPath);
  bool pullFolderRecursively(std::string rootPath, std::string fullPath);
  std::string replaceSubstring(std::string subject, const std::string& search,const std::string& replace);
  bool hasSymlinkLoop(boost::filesystem::path path);

  std::string mDestPort;
  std::string mDestUser;
  std::string mDestHost;
  std::string mDestFolder;
  std::map<unsigned, std::string> mSymlinks;
  WebdavClient mWebdavClient;

};

#endif /* WebdavSyncManager_H */
