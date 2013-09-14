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
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>

#include "Profile.h"

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
  std::vector<Profile>* getProfiles();
 private:
  void createProfile(std::vector<char> name);
  void setSyncType(SyncType syncType);
  void setSyncFolder(std::vector<char> syncFolder);
  void setSyncProtocol( std::vector<char> syncProto );
  void setDestUser(std::vector<char> destUser);
  void setDestHost( std::vector<char> destHost);
  void setDestPort(std::vector<char> port);
  void setDestFolder(std::vector<char> destFolder);
  void setDestPass(std::vector<char> sshPort);
  void pushIgnoredFolder(std::vector<char> ignoredFolder);
  void setSshPort(std::vector<char> sshPort);
  std::vector<Profile> *mpProfiles;
};

#endif /* ConfigFileParser_H */

