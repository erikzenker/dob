#ifndef CommandLineParser_H
#define CommandLineParser_H

#include <string>
#include <boost/spirit/include/classic_core.hpp>

using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;

/**
 * @brief Parses options from the commandline with help of boost::spirit
 *
 **/
class CommandLineParser{
 public:
  CommandLineParser();
  bool parseCommandLine(int argc, char *argv[]);
  string getConfigFileName() const;
 private:
  string mConfigFileName;
};

#endif /* CommandLineParser_H */

