
#pragma once
#include <boost/filesystem.hpp>

#include <Profile.h> /* ProfileData */

/**
 * @brief Parse configfile.
 *
 * All information parsed from Configfile will be filled into a profile
 * object. With the help of this information, the whole profile object
 * can be generated later by the profileFactory.
 *
 **/
class ConfigFileParser{
 public:
  ConfigFileParser();
  ~ConfigFileParser();
  std::vector<ProfileData> parseConfigFile(boost::filesystem::path configFileName);

};



