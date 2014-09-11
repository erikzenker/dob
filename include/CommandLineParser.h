/** 
 * @file      CommandLineParser.h
 * @author    Erik Zenker
 * @date      28.04.2012
 * @copyright Gnu Public License
 */

#pragma once

#include <string>

/**
 * @brief Parses parameters from the commandline with help 
 *        of boost::programm_options
 * @class CommandLineParser
 *        CommandLineParser.h
 *        "include/CommandLineParser.h"
 *
 */
class CommandLineParser{
 public:
  CommandLineParser();
  ~CommandLineParser();
  bool parseCommandLine(unsigned argc, char *argv[]);
  std::string getConfigFileName() const;
  unsigned getDebugLevel() const;

 private:
  std::string mConfigFileName;
  unsigned mDebugLevel;
};


