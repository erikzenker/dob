/** 
 * @file      LocalSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef LocalSyncManager_H
#define LocalSyncManager_H

#include <string>
#include <SyncManager.h>
#include <FileStateDatabase.h>

/**
 * @brief Syncronization with local data
 * @class LocalSyncManager
 *        LocalSyncManager.h
 *        "include/LocalSyncManager.h"
 * 
 * For local syncronization common unix tools
 * are used (cp, rm)
 *
 **/
class LocalSyncManager : public SyncManager {

public:
  LocalSyncManager(boost::filesystem::path destFolder, SyncType syncType);
  virtual bool syncInitial(boost::filesystem::path sourceFolder);
  virtual bool syncFolder(boost::filesystem::path sourceFolder, boost::filesystem::path syncFolder);
  virtual bool syncFile(boost::filesystem::path sourceFolder, boost::filesystem::path syncFolder, ModState ms);
  virtual bool removeFolder(boost::filesystem::path sourceFolder, boost::filesystem::path syncFolder);

 protected: 
  std::string mRsyncOptions;

};


#endif /* LocalSyncManager_H */
