#include <CommandLineParser.h>

#include <string>
#include <dbg_print.h>
#include <iostream>

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
  namespace po = boost::program_options;

  bool matched = true;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("config,c", po::value(&mConfigFileName), "set config file")
    ("debug,d", po::value<unsigned>(&mDebugLevel), "set debug level (0-5)\n\n");

  po::variables_map vm;        
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);   

  if(!vm.count("config")){
    matched &= false;
  }

  if(vm.count("help") or !matched){
    std::cout << "Usage: ./dob [options]" << std::endl;
    std::cout << std::endl;
    std::cout << desc << std::endl;
    matched &= false;
  }

  return matched;

 

}

std::string CommandLineParser::getConfigFileName() const{
  return mConfigFileName;
}

unsigned CommandLineParser::getDebugLevel() const{
  return mDebugLevel;
}

