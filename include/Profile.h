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
  ~Profile();
  // Setter
  void setName(vector<char> name)                 { this->name.assign(name.begin(), name.end());}
  void setSyncType(vector<char> syncType)         { this->syncType.assign(syncType.begin(), syncType.end());}
  void setSyncType(string syncType)               { this->syncType = syncType;}
  void setSyncFolder(vector<char> syncFolder)     { this->syncFolder.assign(syncFolder.begin(), syncFolder.end());}
  void setDestFolder(vector<char> destFolder)     { this->destFolder.assign(destFolder.begin(), destFolder.end());}
  void setDestType(vector<char> destType)         { mDestType.assign(destType.begin(), destType.end());}
  void setDestProtocol(vector<char> destProtocol) { mDestProtocol.assign(destProtocol.begin(), destProtocol.end());}
  void setDestType(string destType)               { mDestType = destType;}
  void setSyncManager(SyncManager* syncManager)   { mpSyncManager = syncManager;}
  void setEventManager(EventManager* eventManager){ mpEventManager = eventManager;}
  void setFileSystemScanner(FileSystemScanner* fileSystemScanner) { mpFileSystemScanner = fileSystemScanner;}
  // Getter
  string getName()         { return name;}
  string getSyncType()     { return syncType;}
  string getSyncFolder()   { return syncFolder;}
  string getDestFolder()   { return destFolder;}
  string getDestType()     { return mDestType;}
  string getDestProtocol() { return mDestProtocol;}
  SyncManager *getSyncManager() { return mpSyncManager;}
  EventManager *getEventManager() { return mpEventManager;}
  FileSystemScanner *getFileSystemScanner() { return mpFileSystemScanner;}
  // Auxillary functions
  void print();
  bool isValid();
  bool startProfile();
  bool stopProfile();
  bool startSyncProfile();
  bool startScanProfile();
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
