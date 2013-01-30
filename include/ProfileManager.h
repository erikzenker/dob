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
  bool startProfile(std::string profileName);
  bool stopProfile(std::string profileName);
  bool restartProfile(std::string profileName);

 private:
  Profile* getProfileByName(std::string profileName);
  vector<Profile>* mpProfiles;

};

#endif /* ProfileManager_H */
