/* 
 * @file      ProfileFactory.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 */

#ifndef PROFILEFACTORY_H
#define PROFILEFACTORY_H

#include <Profile.h>
#include <dbg_print.h>
#include <vector>
/**
 * @brief Generates complete profiles depending on configuration
 *
 **/
class ProfileFactory {
 public:
  ProfileFactory();
  Profile* MakeProfile(Profile* profile);
  vector<Profile>* MakeProfiles(vector<Profile>* pProfiles);
};

#endif /* PROFILEFACTORY_H */
