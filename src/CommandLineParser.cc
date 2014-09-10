#include <CommandLineParser.h>

#include <string>
#include <dbg_print.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

// Parse commandline with boost::programm_options
// http://www.boost.org/doc/libs/1_56_0/doc/html/program_options.html
#include <boost/program_options.hpp>

CommandLineParser::CommandLineParser() :
  mConfigFileName(""),
  mDebugLevel(LOG_INFO){

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
bool CommandLineParser::parseCommandLine(unsigned argc, char *argv[]){
  dbg_printc(LOG_INFO, "CommandLineParser", "ParseCommandLine", "Parse commandline");
  namespace po = boost::program_options;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("config,c", po::value(&mConfigFileName), "set config file")
    ("debug,d", po::value<unsigned>(&mDebugLevel), "set debug level (0-5)\n\n");

  po::variables_map vm;        
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);   

  if(vm.count("config")){
    return true;
  }

  if(vm.count("help")){
    std::cout << "Usage: ./dob [options]" << std::endl;
    std::cout << std::endl;
    std::cout << desc << std::endl;
  }

  return false;

  // namespace qi = boost::spirit::qi;  
  // namespace ascii = boost::spirit::ascii;
  // using ascii::space;
  // bool matched = false;

  // // ConfigFileName Rule
  // qi::rule<char const*>                configFileNameKey   = qi::string("--config=");
  // qi::rule<char const*, std::string()> configFileNameValue = (*qi::char_);

  // // Debug Level Rule (unsigned between 0 and 5)
  // qi::rule<char const*>             debugLevelKey   = qi::string("-d=");
  // qi::rule<char const*, unsigned()> debugLevelValue = qi::int_parser<int, 10, 0, 5>();
  
  // for(unsigned i = 1; i < argc; ++i){
  //   char const* begin(argv[i]);
  //   char const* end(begin + strlen(begin));
  //   matched = false;

  //   matched = matched || qi::parse(begin, end, configFileNameKey >> configFileNameValue, mConfigFileName);
  //   matched = matched || qi::parse(begin, end, debugLevelKey >> debugLevelValue, mDebugLevel);
  //   if(!matched) 
  //     dbg_printc(LOG_ERR, "CommandLineParser", "parseCommandLine", "Unknown parameter %s", argv[i]); 

  // }
  // return matched;

}

std::string CommandLineParser::getConfigFileName() const{
  return mConfigFileName;
}

unsigned CommandLineParser::getDebugLevel() const{
  return mDebugLevel;
}
