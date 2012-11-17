#include <ProfileManager.h>

ProfileManager::ProfileManager(std::vector<Profile>* pProfiles) : 
  mpProfiles(pProfiles){
  
}

ProfileManager::~ProfileManager(){

}

bool ProfileManager::StartProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    dbg_printc(LOG_DBG, "ProfileManager", "StartProfile", "Start profile [%s]", profileName.c_str());
    pProfile->GetFileSystemScanner()->StartToScan();
    return true;
  }
    
  return false;
}

bool ProfileManager::StopProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    dbg_printc(LOG_DBG, "ProfileManager", "StartProfile", "Start profile [%s]", profileName.c_str());
    pProfile->GetFileSystemScanner()->StopToScan();
    return true;
  }
    
  return false;

}

bool ProfileManager::RestartProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    dbg_printc(LOG_DBG, "ProfileManager", "StartProfile", "Start profile [%s]", profileName.c_str());
    pProfile->GetFileSystemScanner()->StopToScan();
    pProfile->GetFileSystemScanner()->StartToScan();
    return true;
  }
    
  return false;

}

Profile* ProfileManager::GetProfileByName(std::string profileName){
  int i;
  for(i = 0; i < mpProfiles->size(); ++i){
    if(!mpProfiles->at(i).GetName().compare(profileName)){
      return &(mpProfiles->at(i));
    }
  }
  return NULL;
}
