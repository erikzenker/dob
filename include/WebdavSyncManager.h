/** 
 * @file      WebdavSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef WebdavSyncManager_H
#define WebdavSyncManager_H

#include <string>
#include <boost/filesystem.hpp> /* filesystem, is_directory, file_size, last_write_time */
#include <map> /* map */
#include <SyncManager.h>
#include <WebdavClient.h>
#include <SyncType.h>
#include <FileStateDatabase.h>

/**
 * @brief Syncronization with remote host.
 * @class WebdavSyncManager
 *        WebdavSyncManager.h
 *        "include/WebdavSyncManager.h"
 * 
 **/

class WebdavSyncManager : public SyncManager {
public:
  WebdavSyncManager(boost::filesystem::path scanPath, std::string destFolder, SyncType syncType,  std::string destUser, std::string destHost, std::string destPort, std::string destPass);
  virtual bool syncInitial(boost::filesystem::path rootPath);
  virtual bool syncFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath);
  virtual bool syncFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms);
  virtual bool removeFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath);

private:
  bool pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms);
  bool pushFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms);
  bool pushFolderRecursively(boost::filesystem::path rootPath, boost::filesystem::path fullPath, bool checkExistance);
  bool pullFolderRecursively(boost::filesystem::path rootPath, std::string uri);
  std::string replaceSubstring(std::string subject, const std::string& search,const std::string& replace);
  bool hasSymlinkLoop(boost::filesystem::path path);

  std::map<unsigned, std::string> mSymlinks;
  WebdavClient mWebdavClient;
  FileStateDatabase mFileStateDatabase;

};

#endif /* WebdavSyncManager_H */
