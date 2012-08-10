#include "ConfigFileParser.h"

void ConfigFileParser::parseConfigFile(string configFileName){
  cerr << "\nC Parse config file: " << configFileName;
  string line;
  ifstream config_file_stream;
  config_file_stream.open(configFileName.c_str());


  if(config_file_stream.is_open())
  {
    while(config_file_stream.good())
    {
      getline (config_file_stream,line);
      for(int i = 0; i < keyWords.size(); i++){
	
	parse(line.c_str(),str_p(keyWords[i].c_str()) >> *space_p >> ch_p('=') >> *space_p >> (*anychar_p)[&print][&push_back],space_p);

      }

    }
    config_file_stream.close();

  }
  else cerr << "\nC Unable to open file: "<< configFileName; 
  config_file_stream.close();
}

void ConfigFileParser::addKeyWord(string keyWord){
  keyWords.push_back(keyWord);
}

void ConfigFileParser::getValue(string keyWord){

}
