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
 * @param  argc   number of parameters
 * @param  argv   list of parameters
 * @return        if there were some parameters
 *
 */
bool CommandLineParser::parseCommandLine(int argc, char *argv[]){
  dbg_printc(LOG_INFO, "CommandLineParser", "ParseCommandLine", "Parse commandline");
  namespace qi = boost::spirit::qi;  
  namespace ascii = boost::spirit::ascii;
  using ascii::space;
  bool matched = false;

  // ConfigFileName Rule
  qi::rule<char const*>                configFileNameKey   = qi::string("--config=");
  qi::rule<char const*, std::string()> configFileNameValue = (*qi::char_);

  // UseGui Rule
  qi::rule<char const*> useGuiKey = qi::string("--usegui");

  // Debug Level Rule (unsigned between 0 and 5)
  qi::rule<char const*>             debugLevelKey   = qi::string("-d=");
  qi::rule<char const*, unsigned()> debugLevelValue = qi::int_parser<int, 10, 0, 5>();
  
  unsigned debugLevel = 0;
  unsigned i;
  for(int i = 1; i < argc; ++i){
    char const* begin(argv[i]);
    char const* end(begin + strlen(begin));
    matched = false;

    matched = matched || qi::parse(begin, end, configFileNameKey >> configFileNameValue, mConfigFileName);
    matched = matched || qi::parse(begin, end, debugLevelKey >> debugLevelValue, mDebugLevel);
    mUseGui = qi::parse(begin, end, useGuiKey);
    if(!matched) 
      dbg_printc(LOG_ERR, "CommandLineParser", "parseCommandLine", "Unknown parameter %s", argv[i]); 

  }
  return matched;

}

std::string CommandLineParser::getConfigFileName() const{
  return mConfigFileName;
}

unsigned CommandLineParser::getDebugLevel() const{
  return mDebugLevel;
}


bool CommandLineParser::getUseGui() const{
  return mUseGui;
}
