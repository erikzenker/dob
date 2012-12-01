#include <ProfileFactory.h>

ProfileFactory::ProfileFactory(){

}

bool ProfileFactory::MakeProfile(Profile* profile){
  string scanFolder = profile->GetSyncFolder();
  string destFolder = profile->GetDestFolder();
  string syncType   = profile->GetSyncType();
  string destType   = profile->GetDestType();
  string destProtocol = profile->GetDestProtocol();
  EventManager* pEventManager;
  SyncManager* pSyncManager;
  FileSystemScanner* pFileSystemScanner;
  
  if(!destType.compare("remote")){
    if(!destProtocol.compare("ssh")){
      pSyncManager = new RemoteSyncManager(destFolder, syncType, destProtocol);
    }
    else if(!destProtocol.compare("git")){
      pSyncManager = new GitSyncManager(destFolder, syncType, destProtocol);
    }
  }
  else if(!destType.compare("local")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
  }
  else{
    dbg_print(LOG_FATAL,"\nC destType %s in configfile is not an option", destType.c_str());
    return false;
  }

  pEventManager = new OptimizedEventManager(pSyncManager);
  pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, pEventManager);
  profile->SetSyncManager(pSyncManager);
  profile->SetEventManager(pEventManager);
  profile->SetFileSystemScanner(pFileSystemScanner);
  return true;

}

bool ProfileFactory::MakeProfiles(vector<Profile>* pProfiles){
  for(int i=0; i < pProfiles->size(); ++i){
    if(pProfiles->at(i).IsValid()){
      if(!MakeProfile(&(pProfiles->at(i)))){
	return false;
      }

    }
    else{
      return false;
    }

  }

  return true;
}
