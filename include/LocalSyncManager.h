/** 
 * @file      LocalSyncManager.h
 * @author    Erik Zenker
 * @date      30.01.2013
 * @copyright Gnu Public License
 */

#ifndef LocalSyncManager_H
#define LocalSyncManager_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dbg_print.h>

#include "SyncManager.h"

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
  LocalSyncManager(std::string destFolder, std::string syncType);
  virtual bool syncSourceFolder(std::string sourceFolder);
  virtual bool syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder);
  virtual bool syncFile(std::string sourceFolder, std::string syncFolder);
  virtual bool removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder);

 protected: 
  virtual bool checkDestFolder();
  virtual bool mountDestFolder();

};


#endif /* LocalSyncManager_H */
