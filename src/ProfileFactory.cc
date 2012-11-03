#include <ProfileFactory.h>

ProfileFactory::ProfileFactory(){

}

Profile* ProfileFactory::MakeProfile(Profile* profile){
  string scanFolder = profile->GetSyncFolder();
  string destFolder = profile->GetDestFolder();
  string syncType   = profile->GetSyncType();
  string destLocation = profile->GetDestLocation();

  SyncManager* pSyncManager;
  
  if(!destLocation.compare("remote")){
    pSyncManager = new RemoteSyncManager(destFolder, syncType);
  }
  else if(!destLocation.compare("local")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
  }
  else{
    dbg_print(LOG_FATAL,"\nC destLocation %s in configfile is not an option", destLocation.c_str());
    return NULL;
  }

  EventManager* pEventManager = new OptimizedEventManager(pSyncManager);
  FileSystemScanner* pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, pEventManager);
  profile->SetSyncManager(pSyncManager);
  profile->SetEventManager(pEventManager);
  profile->SetFileSystemScanner(pFileSystemScanner);
  return profile;

}

vector<Profile>* ProfileFactory::MakeProfiles(vector<Profile>* pProfiles){
  for(int i=0; i < pProfiles->size(); ++i){
    if(pProfiles->at(i).IsValid()){
      if(!MakeProfile(&(pProfiles->at(i)))){
	return NULL;
      }

    }

  }

  return pProfiles;
}
