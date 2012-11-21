#include <ProfileFactory.h>

ProfileFactory::ProfileFactory(){

}

bool ProfileFactory::MakeProfile(Profile* profile){
  string scanFolder = profile->GetSyncFolder();
  string destFolder = profile->GetDestFolder();
  string syncType   = profile->GetSyncType();
  string destLocation = profile->GetDestLocation();
  string mountOptions = profile->GetMountOptions();
  EventManager* pEventManager;
  SyncManager* pSyncManager;
  FileSystemScanner* pFileSystemScanner;
  
  if(!destLocation.compare("remote")){
    pSyncManager = new RemoteSyncManager(destFolder, syncType, mountOptions);
  }
  else if(!destLocation.compare("local")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
  }
  else{
    dbg_print(LOG_FATAL,"\nC destLocation %s in configfile is not an option", destLocation.c_str());
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
