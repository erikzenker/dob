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
 * @brief Parses key/value pairs of configfile with help of boost::spirit
 *
 **/
class ConfigFileParser{
 public:
  ConfigFileParser();
  void ParseConfigFile(string configFileName);
  vector<Profile>* GetProfiles();
 private:
  void CreateProfile(vector<char> name);
  void SetSyncType(string syncType);
  void SetSyncFolder(vector<char> syncFolder);
  void SetDestFolder(vector<char> destFolder);
  void SetDestLocation(string destLocation);
  void SetMountOptions(vector<char> mountPoints);
  vector<Profile> *mpProfiles;
};

#endif /* ConfigFileParser_H */

