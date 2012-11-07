#include "CommandLineParser.h"

CommandLineParser::CommandLineParser() :
  mDebugLevel(LOG_INFO),
  mConfigFileName(""){

}

/**
 * @todo update spirit parser to new version 2.5 (use of qi)
 **/
bool CommandLineParser::ParseCommandLine(int argc, char *argv[]){
  dbg_printc(LOG_INFO, "CommandLineParser", "ParseCommandLine", "Parse commandline");
  rule<phrase_scanner_t> config;
  rule<phrase_scanner_t> debug;
  bool matched = false;

  config = str_p("--config=")>>(*anychar_p)[assign_a(mConfigFileName)][assign_a(matched,true)];
  debug = str_p("-d=")>>(int_p)[assign_a(mDebugLevel)];
  for(int i = 1; i < argc; ++i){
    parse(argv[i],(*config),space_p);
    parse(argv[i],(*debug),space_p);

  }

  return matched;

}

string CommandLineParser::GetConfigFileName() const{
  return mConfigFileName;
}

int CommandLineParser::GetDebugLevel() const{
  return mDebugLevel;
}



