#include <ProfileFactory.h>
#include <WebdavSyncManager.h>
#include <LocalSyncManager.h>
#include <InotifyFileSystemScanner.h>
#include <FolderEventManager.h>
#include <FileEventManager.h>

#include <iostream>

ProfileFactory::ProfileFactory(){

}

ProfileFactory::~ProfileFactory(){

}


/**
 * @brief Manipulates one profile object.
 *
 * Depending on the information given by the profile,
 * the right objects will be instanciated to create
 * the full profile.
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
  std::string profileName =  profile->getName();
  SyncType syncType   = profile->getSyncType();
  std::string scanFolder = profile->getSyncFolder();
  std::string syncProtocol = profile->getSyncProtocol();

  std::string destUser	 = profile->getDestUser();
  std::string destHost	 = profile->getDestHost();
  std::string destPort	 = profile->getDestPort();
  std::string destFolder = profile->getDestFolder();
  std::string destPass   = profile->getDestPass();
  std::string sshPort    = profile->getSshPort();

  EventManager* pEventManager;
  SyncManager* pSyncManager;
  FileSystemScanner* pFileSystemScanner;
  std::vector<std::string> ignoredFolders = profile->getIgnoredFolders();
  int eventTimeout = 0;
  
  if(!profileName.compare(""))
    return false;
// if local transfer is used, this assumes remote, before it checks for local!
// TODO revise!
  if(!syncProtocol.compare("webdav")){
    pSyncManager = new WebdavSyncManager(destFolder, syncType, destUser, destHost, destPort, destPass);
    eventTimeout = 0;
  }
  else if(!destHost.compare("") && !syncProtocol.compare("")){
    pSyncManager = new LocalSyncManager(destFolder, syncType);
    eventTimeout = 1;
  }
  else{
    dbg_printc(LOG_FATAL,"ProfileFactory","makeProfile","syncProtocol %s in configfile is not an option", syncProtocol.c_str());
    return false;
  }

  if(!syncProtocol.compare("webdav")){
    pEventManager = new FileEventManager(pSyncManager);
  }
  else{
    pEventManager = new FolderEventManager(pSyncManager);
  }

  pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, ignoredFolders, eventTimeout, pEventManager);

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
bool ProfileFactory::makeProfiles(std::vector<Profile>* pProfiles){
  for(unsigned i = 0; i < pProfiles->size(); ++i){
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
