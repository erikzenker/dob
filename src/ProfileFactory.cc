#include <ProfileFactory.h>

ProfileFactory::ProfileFactory(){

}

ProfileFactory::~ProfileFactory(){

}


/**
 * @brief Manipulates one profile object.
 *
 * Depending on the information given by the profile,
 * the right objects will be instanciated to create
 * the wanted profile.
 * If there are some Information missing, for example
 * profileName or destType, the method will return
 * false and won't manipulate the profile object.
 *
 * @param  Profile you want to instanciate
 * @return true  the Profile was instanciated right
 * @return false there are information missing
 *               in the profile information
 **/
bool ProfileFactory::makeProfile(Profile* profile){
  string profileName =  profile->getName();
  string scanFolder = profile->getSyncFolder();
  string destFolder = profile->getDestFolder();
  string syncType   = profile->getSyncType();
  string destType   = profile->getDestType();
  string destProtocol = profile->getDestProtocol();
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
    else{
      dbg_printc(LOG_FATAL,"ProfileFactory", "makeProfile","Please specifiy destProtocol of destType", destType.c_str());
      return false;

    }
  }
  else if(!destType.compare("local")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
    eventTimeout = 1;
  }
  else{
    dbg_printc(LOG_FATAL,"ProfileFactory","makeProfile","destType %s in configfile is not an option", destType.c_str());
    return false;
  }

  pEventManager = new OptimizedEventManager(pSyncManager);
  pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, ignoredFolder, eventTimeout, pEventManager);

  profile->setSyncManager(pSyncManager);
  profile->setEventManager(pEventManager);
  profile->setFileSystemScanner(pFileSystemScanner);
  return true;

}

/**
 * @brief Manipulates a list of profiles.
 * @see    makeProfile
 * 
 * @param  list of profiles
 * @return true  if all profiles were instanciated
 * @return false if there were some information
 *               missing in at least one profile 
 *
 **/
bool ProfileFactory::makeProfiles(vector<Profile>* pProfiles){
  for(int i = 0; i < pProfiles->size(); ++i){
    if(pProfiles->at(i).isValid()){
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
