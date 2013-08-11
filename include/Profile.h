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
  void setName( std::string name )		       { this->name = name; }

  void setSyncType(std::vector<char> syncType)         { this->mSyncType.assign(syncType.begin(), syncType.end());}
  void setSyncType(std::string syncType)               { this->mSyncType = syncType;}

  void setSyncFolder(std::vector<char> syncFolder)     { this->mSyncFolder.assign(syncFolder.begin(), syncFolder.end());}
  void setSyncFolder( std::string syncFolder )	       { this->mSyncFolder = syncFolder; }

  void setSyncProtocol(std::vector<char> syncProtocol) { mSyncProto.assign(syncProtocol.begin(), syncProtocol.end());}
  void setSyncProtocol( std::string syncProtocol )     { mSyncProto = syncProtocol; }
  
  void setDestUser(std::vector<char> destUser)         { mDestUser.assign(destUser.begin(), destUser.end());}
  void setDestUser(std::string destUser)               { mDestUser = destUser;}

  void setDestHost( std::string host )		       { mDestHost = host; }

  void setDestPort(std::string port) {mDestPort = port; }
  
  void setDestFolder(std::vector<char> destFolder)     { mDestFolder.assign(destFolder.begin(), destFolder.end());}
  void setDestFolder( std::string destFolder)	       { mDestFolder = destFolder; }
  void setDestPass(std::string destPass)               { mDestPass = destPass;}

  void setSshPort( std::string port )		       { mSshPort = port; }

  void setSyncManager(SyncManager* syncManager)	       { mpSyncManager = syncManager;}
  void setEventManager(EventManager* eventManager)     { mpEventManager = eventManager;}
  void setFileSystemScanner(FileSystemScanner* fileSystemScanner) { mpFileSystemScanner = fileSystemScanner;}

  void pushIgnoredFolder(std::string ignoredFolder)    { mIgnoredFolders.push_back(ignoredFolder); }

  // Getter
  std::string getName()         { return name;}
  std::string getSyncType()     { return mSyncType;}
  std::string getSyncFolder()   { return mSyncFolder;}
  std::string getSyncProtocol() { return mSyncProto;}
  std::string getDestUser()	{ return mDestUser; }
  std::string getDestHost()	{ return mDestHost; }
  std::string getDestPort()	{ return mDestPort; }
  std::string getDestFolder()   { return mDestFolder; }
  std::string getDestPass()     { return mDestPass; }
  std::string getSshPort()	{ return mSshPort; }

  SyncManager *getSyncManager() { return mpSyncManager;}
  EventManager *getEventManager() { return mpEventManager;}
  FileSystemScanner *getFileSystemScanner() { return mpFileSystemScanner;}
  std::vector<std::string> getIgnoredFolders() { return mIgnoredFolders;}
  
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
  std::string mSyncProto;
  std::string mDestUser;
  std::string mDestHost;
  std::string mDestPort;
  std::string mDestFolder;
  std::string mDestPass;
  std::string mSshPort;
  
  std::vector<std::string> mIgnoredFolders;
  SyncManager * mpSyncManager;
  EventManager * mpEventManager;
  FileSystemScanner * mpFileSystemScanner;

};

#endif /* Profile_H */
