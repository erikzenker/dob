/* 
 * @file      ProfileFactory.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 */

#pragma once
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include <Profile.h>
#include <dbg_print.h>


/**
 * @brief Generates complete profiles depending on configuration.
 * @class ProfileFactory
 *        ProfileFactory.h
 *        "include/ProfileFactory.h"
 *
 * The ProfileFactory can't create profiles from the scratch. It
 * needs a prefilled profile with information about what
 * FileSystemScanner, EventManager and SyncManager to create.
 * Usually this information is given by the ConfigFileParser.
 *
 **/
class ProfileFactory {
 public:
  ProfileFactory();
  ~ProfileFactory();
  std::vector<Profile> makeProfiles(boost::filesystem::path configFileName);
 private:
  Profile makeProfile(ProfileData profileData);
};

