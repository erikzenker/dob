#ifndef ConfigFileParser_H
#define ConfigFileParser_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <dbg_print.h>

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
  void parseConfigFile(string configFileName);
  vector<Profile>* getProfiles();
 private:
  void createProfile(vector<char> name);
  void setSyncType(string syncType);
  void setSyncFolder(vector<char> syncFolder);
  void setDestFolder(vector<char> destFolder);
  void setDestType(string destLocation);
  void setDestProtocol(vector<char> destProtocol);
  void pushIgnoredFolder(vector<char> destProtocol);
  vector<Profile> *mpProfiles;
};

#endif /* ConfigFileParser_H */

