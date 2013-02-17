#ifndef ConfigFileParser_H
#define ConfigFileParser_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <dbg_print.h>
#include <stdlib.h>

// phrase_parse includes
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>

#include "Profile.h"

using namespace std;

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
  void parseConfigFile(std::string configFileName);
  vector<Profile>* getProfiles();
 private:
  void createProfile(std::vector<char> name);
  void setSyncType(std::string syncType);
  void setSyncFolder(std::vector<char> syncFolder);
  void setDestFolder(std::vector<char> destFolder);
  void setDestType(std::string destLocation);
  void setDestProtocol(std::vector<char> destProtocol);
  void pushIgnoredFolder(std::vector<char> destProtocol);
  void setDestPort(std::vector<char> port);
  std::vector<Profile> *mpProfiles;
};

#endif /* ConfigFileParser_H */

