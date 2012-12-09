/**
 * @file      ProfileManager.h
 * @author    Erik Zenker
 * @date      15.11.2012
 * @copyright Gnu Public License
 **/
#ifndef ProfileManager_H
#define ProfileManager_H

#include <vector>
#include <string>
#include <dbg_print.h>
#include <Profile.h>

class ProfileManager {
 public:
  ProfileManager(std::vector<Profile>* pProfiles);
  ~ProfileManager();
  bool StartProfile(std::string profileName);
  bool StopProfile(std::string profileName);
  bool RestartProfile(std::string profileName);

 private:
  Profile* GetProfileByName(std::string profileName);
  vector<Profile>* mpProfiles;

};

#endif /* ProfileManager_H */
