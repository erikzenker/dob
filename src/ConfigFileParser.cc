#include "ConfigFileParser.h"

void ConfigFileParser::parseConfigFile(string configFileName){
  cerr << "\nC Parse config file: " << configFileName;
  string line;
  ifstream config_file_stream;
  config_file_stream.open(configFileName.c_str());
  parsedValues.resize(keyWords.size(),"");

  vector<string> profiles;

  if(config_file_stream.is_open())
  {
    while(config_file_stream.good())
    {
      getline (config_file_stream,line);

      //bool r = phrase_parse(line.begin(),line.end(),(*anychar_p),space_p);
      //parse(line.c_str(),'[' >> (*anychar_p)[push_back_a(profiles)] >> ']', space_p);
      //cout << "\n C profile: "<<profiles.back();
      
      
      for(int i = 0; i < keyWords.size(); i++){
	parse(line.c_str(),str_p(keyWords[i].c_str()) >> *space_p >> ch_p('=') >> *space_p >> (*anychar_p)[assign_a(parsedValues[i])],space_p);


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

string ConfigFileParser::getValue(string keyWord){
  assert(keyWords.size() == parsedValues.size());
  for(int i = 0; i < keyWords.size(); i++){
    if(!keyWord.compare(keyWords[i]))
	return parsedValues[i];

  }
  return 0;
}
