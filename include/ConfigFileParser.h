#ifndef ConfigFileParser_H
#define ConfigFileParser_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_push_back_actor.hpp>

using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;


namespace
{
  vector<string> parsedValues;
  vector<string> keyWords;
/* Definition of semantic actions, but
   is not needed anymore. Use now
   predefined semantic actions from 
   spirit parser framework

  void print(char const* first, char const* last)
  {
    string str(first, last);
    cout << "\n" << str ;
  }

  void push_back(char const* first, char const* last)
  {
    string str(first, last);
    parsedValues.push_back(str);
  }
*/
}

/**
 * @brief Parses key/value pairs of configfile with help of boost::spirit
 *
 * The Configfile should exist out of key value pairs like
 * KEY = VALUE. You can add keys you want to parse with the
 * addkeyWord(string key) method and get the value with getValue(string key)
 * after you parsed the configfile with parseConfigfile(string configFileName).
 *
 **/
class ConfigFileParser{
 public:
  void parseConfigFile(string configFileName);
  void addKeyWord(string keyWord);
  string getValue(string keyWord);
};

#endif /* ConfigFileParser_H */

