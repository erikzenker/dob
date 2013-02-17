/* 
 * @file      ProfileFactory.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 */

#ifndef PROFILEFACTORY_H
#define PROFILEFACTORY_H

#include <iostream>
#include <vector>

#include <Profile.h>
#include <GitSyncManager.h>
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
  bool makeProfiles(std::vector<Profile>* pProfiles);
 private:
  bool makeProfile(Profile* profile);
};

#endif /* PROFILEFACTORY_H */
