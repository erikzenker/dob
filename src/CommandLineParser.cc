#include "CommandLineParser.h"

CommandLineParser::CommandLineParser(){
  

}

/**
 * @todo update spirit parser to new version 2.5 (use of qi)
 **/
bool CommandLineParser::parseCommandLine(int argc, char *argv[]){
  cerr << "\nC Parse commandline";
  rule<phrase_scanner_t> args;
  bool matched = false;

  args = str_p("--config=")>>(*anychar_p)[assign_a(mConfigFileName)][assign_a(matched,true)];
  for(int i = 1; i < argc; ++i){
    parse(argv[i],(*args),space_p);

  }

  return matched;

}

string CommandLineParser::getConfigFileName() const{
  return mConfigFileName;
}



