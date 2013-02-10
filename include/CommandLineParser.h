/** 
 * @file      CommandLineParser.h
 * @author    Erik Zenker
 * @date      28.04.2012
 * @copyright Gnu Public License
 */

#ifndef CommandLineParser_H
#define CommandLineParser_H

#include <string>
#include <dbg_print.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>
#include <iostream>

/**
 * @brief Parses parameters from the commandline with help 
 *        of boost::spirit framework.
 * @class CommandLineParser
 *        CommandLineParser.h
 *        "include/CommandLineParser.h"
 *
 * Boost::spirit was choosen, because it is easy to write
 * new parser rules. So new feature can be added easily.
 * Also there is no need to write a full parser again
 * and again for each new project you have. Spirit gives
 * you all tools to build a powerfull parser for your
 * system.
 *
 */
class CommandLineParser{
 public:
  CommandLineParser();
  ~CommandLineParser();
  bool parseCommandLine(unsigned argc, char *argv[]);
  std::string getConfigFileName() const;
  unsigned getDebugLevel() const;
  bool getUseGui() const;

 private:
  std::string mConfigFileName;
  unsigned mDebugLevel;
  bool mUseGui;
};

#endif /* CommandLineParser_H */

