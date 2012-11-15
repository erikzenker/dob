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
  void SetDestLocation(vector<char> destLocation) { this->destLocation.assign(destLocation.begin(), destLocation.end());}
  void SetDestLocation(string destLocation)       { this->destLocation = destLocation;}
  void SetSyncManager(SyncManager* syncManager)   { mpSyncManager = syncManager;}
  void SetEventManager(EventManager* eventManager){ mpEventManager = eventManager;}
  void SetFileSystemScanner(FileSystemScanner* fileSystemScanner) { mpFileSystemScanner = fileSystemScanner;}
  // Getter
  string GetName()         { return name;}
  string GetSyncType()     { return syncType;}
  string GetSyncFolder()   { return syncFolder;}
  string GetDestFolder()   { return destFolder;}
  string GetDestLocation() { return destLocation;}
  SyncManager *GetSyncManager() { return mpSyncManager;}
  EventManager *GetEventManager() { return mpEventManager;}
  FileSystemScanner *GetFileSystemScanner() { return mpFileSystemScanner;}
  // Auxillary functions
  void print();
  bool IsValid();

 private:
  string name;
  string syncType;
  string syncFolder;
  string destFolder;
  string destLocation;
  SyncManager * mpSyncManager;
  EventManager * mpEventManager;
  FileSystemScanner * mpFileSystemScanner;

};

#endif /* Profile_H */
