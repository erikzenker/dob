#include <ProfileManager.h>

ProfileManager::ProfileManager(std::vector<Profile>* pProfiles) : 
  mpProfiles(pProfiles){
  
}

ProfileManager::~ProfileManager(){

}

bool ProfileManager::StartProfile(std::string profileName){
  dbg_printc(LOG_DBG, "ProfileManager", "StartProfile", "Start profile [%s]", profileName.c_str());
  return true;
}

bool ProfileManager::StopProfile(std::string profileName){
  dbg_printc(LOG_DBG, "ProfileManager", "StopProfile", "Start profile [%s]", profileName.c_str());
  return true;
}

bool ProfileManager::RestartProfile(std::string profileName){
  dbg_printc(LOG_DBG, "ProfileManager", "RestartProfile", "Start profile [%s]", profileName.c_str());
  return true;
}
