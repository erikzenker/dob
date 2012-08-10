#ifndef CommandLineParser_H
#define CommandLineParser_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <boost/spirit/include/classic_core.hpp>
//#include <boost/spirit.hpp>
using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;

class CommandLineParser{
 public:
  string mConfigFileName;
  void parseCommandLine(int argc, char *argv[]);
  string getConfigFileName();

};

#endif /* CommandLineParser_H */

