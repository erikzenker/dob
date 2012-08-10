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


class ConfigFileParser{
 public:
  void parseConfigFile(string configFileName);
  void addKeyWord(string keyWord);
  string getValue(string keyWord);
};

#endif /* ConfigFileParser_H */

