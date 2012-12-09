/**
 * @file      Profile.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 **/

#ifndef Profile_H
#define Profile_H

#include <string>
#include <iostream>
#include <InotifyFileSystemScanner.h>
#include <RemoteSyncManager.h>
#include <LocalSyncManager.h>
#include <OptimizedEventManager.h>
#include <dbg_print.h>

using namespace std;

/**
 * @brief Stores information about a user profile
 *
 **/
class Profile {
 public:
  Profile();
  // Setter
  void SetName(vector<char> name)                 { this->name.assign(name.begin(), name.end());}
  void SetSyncType(vector<char> syncType)         { this->syncType.assign(syncType.begin(), syncType.end());}
  void SetSyncType(string syncType)               { this->syncType = syncType;}
  void SetSyncFolder(vector<char> syncFolder)     { this->syncFolder.assign(syncFolder.begin(), syncFolder.end());}
  void SetDestFolder(vector<char> destFolder)     { this->destFolder.assign(destFolder.begin(), destFolder.end());}
  void SetDestType(vector<char> destType)         { mDestType.assign(destType.begin(), destType.end());}
  void SetDestProtocol(vector<char> destProtocol) { mDestProtocol.assign(destProtocol.begin(), destProtocol.end());}
  void SetDestType(string destType)               { mDestType = destType;}
  void SetSyncManager(SyncManager* syncManager)   { mpSyncManager = syncManager;}
  void SetEventManager(EventManager* eventManager){ mpEventManager = eventManager;}
  void SetFileSystemScanner(FileSystemScanner* fileSystemScanner) { mpFileSystemScanner = fileSystemScanner;}
  // Getter
  string GetName()         { return name;}
  string GetSyncType()     { return syncType;}
  string GetSyncFolder()   { return syncFolder;}
  string GetDestFolder()   { return destFolder;}
  string GetDestType()     { return mDestType;}
  string GetDestProtocol() { return mDestProtocol;}
  SyncManager *GetSyncManager() { return mpSyncManager;}
  EventManager *GetEventManager() { return mpEventManager;}
  FileSystemScanner *GetFileSystemScanner() { return mpFileSystemScanner;}
  // Auxillary functions
  void print();
  bool IsValid();
  bool StartProfile();
  bool StopProfile();
  bool StartSyncProfile();
  bool StartScanProfile();
 private:
  string name;
  string syncType;
  string syncFolder;
  string destFolder;
  string mDestType;
  string mDestProtocol;
  SyncManager * mpSyncManager;
  EventManager * mpEventManager;
  FileSystemScanner * mpFileSystemScanner;

};

#endif /* Profile_H */
