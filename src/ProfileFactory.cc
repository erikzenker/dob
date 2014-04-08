#include <ProfileFactory.h>
#include <LocalSyncManager.h>
#include <InotifyFileSystemScanner.h>
#include <FileEventManager.h>
#include <ConfigFileParser.h>

ProfileFactory::ProfileFactory(){

}

ProfileFactory::~ProfileFactory(){

}


/**
 * @brief Generates profile from profile data
 *
 **/
Profile ProfileFactory::makeProfile(ProfileData profileData){

  	EventManager* eventManager = nullptr;
  	SyncManager* syncManager = nullptr;
  	FileSystemScanner* fileSystemScanner = nullptr;
  	int eventTimeout = 0;
  
  	if(!profileData.syncProtocol.compare("webdav")){
    		//pSyncManager = new WebdavSyncManager(scanFolder, destFolder, syncType, destUser, destHost, destPort, destPass);
  	}
  	else if(!profileData.destHost.compare("") && !profileData.syncProtocol.compare("")){
    		syncManager = new LocalSyncManager(profileData.destPath, profileData.syncType);
  	}
  	else {
    		throw std::runtime_error("syncProtocol " + profileData.syncProtocol +" in configfile is not an option");
  	}

  
    	eventManager      = new FileEventManager(*syncManager, profileData.ignoredPaths);
  	fileSystemScanner = new InotifyFileSystemScanner(profileData.syncPath, *eventManager, profileData.ignoredPaths, eventTimeout, profileData.profileName);

	Profile profile(*fileSystemScanner, *eventManager, *syncManager);

  	return profile;

}


/**
 * @brief Creates a list of profiles from config file
 * 
 **/
std::vector<Profile> ProfileFactory::makeProfiles(boost::filesystem::path configFileName){
	ConfigFileParser configFileParser;
	std::vector<Profile> profiles;
	std::vector<ProfileData> profileDatas = configFileParser.parseConfigFile(configFileName);
	for(ProfileData profileData : profileDatas){
		profiles.push_back(makeProfile(profileData));
	}

	return profiles;
}
