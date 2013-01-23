#include "CommandLineParser.h"

CommandLineParser::CommandLineParser() :
  mDebugLevel(LOG_INFO),
  mConfigFileName(""),
  mUseGui(false){

}

CommandLineParser::~CommandLineParser(){

}

/**
 * @brief Parses parameters from commandline and makes them
 *        accesable by public getter methods.
 *
 * @todo  update spirit parser to new version 2.5 (use of qi)
 *
 * @param  argc   number of parameters
 * @param  argv   list of parameters
 * @return        if there were some parameters
 *
 */
bool CommandLineParser::parseCommandLine(int argc, char *argv[]){
  dbg_printc(LOG_INFO, "CommandLineParser", "ParseCommandLine", "Parse commandline");
  bool matched = false;
  rule<phrase_scanner_t> config;
  rule<phrase_scanner_t> debug;
  rule<phrase_scanner_t> nogui;


  config = str_p("--config=")>>(*anychar_p)[assign_a(mConfigFileName)][assign_a(matched,true)];

  debug = str_p("-d=")>>(int_p)[assign_a(mDebugLevel)];

  nogui = str_p("--usegui")[assign_a(mUseGui,true)];

  for(int i = 1; i < argc; ++i){
    parse(argv[i],(*config),space_p);
    parse(argv[i],(*debug),space_p);
    parse(argv[i],(*nogui),space_p);

  }

  return matched;

}

string CommandLineParser::getConfigFileName() const{
  return mConfigFileName;
}

int CommandLineParser::getDebugLevel() const{
  return mDebugLevel;
}


bool CommandLineParser::getUseGui() const{
  return mUseGui;
}
