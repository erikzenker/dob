#ifndef CommandLineParser_H
#define CommandLineParser_H

#include <string>
#include <dbg_print.h>
#include <boost/spirit/include/classic_core.hpp>

using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;

/**
 * @brief Parses options from the commandline with help of boost::spirit
 *
 * Boost:.spirit was choosen, because it is easy to write
 * new parser rules. So new feature can be added easily.
 *
 **/
class CommandLineParser{
 public:
  CommandLineParser();
  ~CommandLineParser();
  bool parseCommandLine(int argc, char *argv[]);
  string getConfigFileName() const;
  int getDebugLevel() const;
  bool getNoGui() const;
 private:
  string mConfigFileName;
  int mDebugLevel;
  bool mNoGui;
};

#endif /* CommandLineParser_H */

