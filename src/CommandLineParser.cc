#include "CommandLineParser.h"

void CommandLineParser::parseCommandLine(int argc, char *argv[]){
  cerr << "\nC Parse commandline";
  map<string, string> options;

  rule<phrase_scanner_t> args;

  //args = str_p("--file=")>>(+(alnum_p|chset_p("\\/.~%")))[assign_a(mConfigFileName)];
  args = str_p("--config=")>>(*anychar_p)[assign_a(mConfigFileName)];
  for(int i = 1; i < argc; ++i){
    parse(argv[i],(*args),space_p);

  }


}

string CommandLineParser::getConfigFileName(){
  return mConfigFileName;
}

