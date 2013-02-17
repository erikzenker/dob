/**
 * @file      Profile.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 **/

#ifndef Profile_H
#define Profile_H

#include <string>
#include <vector>
#include <iostream>
#include <InotifyFileSystemScanner.h>
#include <RemoteSyncManager.h>
#include <LocalSyncManager.h>
#include <OptimizedEventManager.h>
#include <dbg_print.h>



/**
 * @brief Stores information about a user profile
 *
 **/
class Profile {
 public:
  Profile();
  ~Profile();
  // Setter
  void setName(std::vector<char> name)                 { this->name.assign(name.begin(), name.end());}
  void setSyncType(std::vector<char> syncType)         { this->mSyncType.assign(syncType.begin(), syncType.end());}
  void setSyncType(std::string syncType)               { this->mSyncType = syncType;}
  void setSyncFolder(std::vector<char> syncFolder)     { this->mSyncFolder.assign(syncFolder.begin(), syncFolder.end());}
  void setDestFolder(std::vector<char> destFolder)     { this->mDestFolder.assign(destFolder.begin(), destFolder.end());}
  void setDestType(std::vector<char> destType)         { mDestType.assign(destType.begin(), destType.end());}
  void setDestProtocol(std::vector<char> destProtocol) { mDestProtocol.assign(destProtocol.begin(), destProtocol.end());}
  void setDestType(std::string destType)               { mDestType = destType;}
  void setSyncManager(SyncManager* syncManager)   { mpSyncManager = syncManager;}
  void setEventManager(EventManager* eventManager){ mpEventManager = eventManager;}
  void setFileSystemScanner(FileSystemScanner* fileSystemScanner) { mpFileSystemScanner = fileSystemScanner;}
  void setDestPort(std::string port) {mDestPort = port;}
  void pushIgnoredFolder(std::string ignoredFolder) { mIgnoredFolders.push_back(ignoredFolder);}
  // Getter
  std::string getName()         { return name;}
  std::string getSyncType()     { return mSyncType;}
  std::string getSyncFolder()   { return mSyncFolder;}
  std::string getDestFolder()   { return mDestFolder;}
  std::string getDestType()     { return mDestType;}
  std::string getDestProtocol() { return mDestProtocol;}
  SyncManager *getSyncManager() { return mpSyncManager;}
  EventManager *getEventManager() { return mpEventManager;}
  FileSystemScanner *getFileSystemScanner() { return mpFileSystemScanner;}
  std::vector<std::string> getIgnoredFolders() { return mIgnoredFolders;}
  std::string getDestPort() { return mDestPort;}
  // Auxillary functions
  void print();
  bool isValid();
  bool startProfile();
  bool stopProfile();
  bool startSyncProfile();
  bool startScanProfile();
 private:
  std::string name;
  std::string mSyncType;
  std::string mSyncFolder;
  std::string mDestFolder;
  std::string mDestType;
  std::string mDestProtocol;
  std::string mDestPort;
  std::vector<std::string> mIgnoredFolders;
  SyncManager * mpSyncManager;
  EventManager * mpEventManager;
  FileSystemScanner * mpFileSystemScanner;

};

#endif /* Profile_H */
