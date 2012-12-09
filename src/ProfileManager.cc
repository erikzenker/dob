#include <ProfileManager.h>

ProfileManager::ProfileManager(std::vector<Profile>* pProfiles) : 
  mpProfiles(pProfiles){
  
}

ProfileManager::~ProfileManager(){

}

bool ProfileManager::StartProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    pProfile->StartProfile();
    return true;
  }
    
  return false;
}

bool ProfileManager::StopProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    pProfile->StopProfile();
    return true;
  }
    
  return false;
}

bool ProfileManager::RestartProfile(std::string profileName){
  Profile * pProfile = GetProfileByName(profileName);
  if(pProfile){
    pProfile->StopProfile();
    pProfile->StartProfile();
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
