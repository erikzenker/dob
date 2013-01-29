#include <ProfileFactory.h>

ProfileFactory::ProfileFactory(){

}

bool ProfileFactory::makeProfile(Profile* profile){
  string profileName =  profile->GetName();
  string scanFolder = profile->GetSyncFolder();
  string destFolder = profile->GetDestFolder();
  string syncType   = profile->GetSyncType();
  string destType   = profile->GetDestType();
  string destProtocol = profile->GetDestProtocol();
  EventManager* pEventManager;
  SyncManager* pSyncManager;
  FileSystemScanner* pFileSystemScanner;
  std::string ignoredFolder = "";
  int eventTimeout = 0;
  
  if(!profileName.compare(""))
    return false;
  
  if(!destType.compare("remote")){
    if(!destProtocol.compare("ssh")){
      pSyncManager = new RemoteSyncManager(destFolder, syncType, destProtocol);
      eventTimeout = 1;
    }
    else if(!destProtocol.compare("git")){
      pSyncManager = new GitSyncManager(destFolder, syncType, destProtocol);
      ignoredFolder = scanFolder;
      ignoredFolder.append(".git");
      eventTimeout = 1;
    }
  }
  else if(!destType.compare("local")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
    eventTimeout = 1;
  }
  else{
    dbg_print(LOG_FATAL,"\nC destType %s in configfile is not an option", destType.c_str());
    return false;
  }

  pEventManager = new OptimizedEventManager(pSyncManager);
  pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, ignoredFolder, eventTimeout, pEventManager);

  profile->SetSyncManager(pSyncManager);
  profile->SetEventManager(pEventManager);
  profile->SetFileSystemScanner(pFileSystemScanner);
  return true;

}

bool ProfileFactory::makeProfiles(vector<Profile>* pProfiles){
  for(int i = 0; i < pProfiles->size(); ++i){
    if(pProfiles->at(i).IsValid()){
      if(!makeProfile(&(pProfiles->at(i)))){
	return false;
      }

    }
    else{
      return false;
    }

  }

  return true;
}
