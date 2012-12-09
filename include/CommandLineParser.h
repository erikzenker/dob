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
 **/
class CommandLineParser{
 public:
  CommandLineParser();
  bool ParseCommandLine(int argc, char *argv[]);
  string GetConfigFileName() const;
  int GetDebugLevel() const;
  bool GetNoGui() const;
 private:
  string mConfigFileName;
  int mDebugLevel;
  bool mNoGui;
};

#endif /* CommandLineParser_H */

