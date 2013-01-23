#include "ConfigFileParser.h"
ConfigFileParser::ConfigFileParser():
  mpProfiles(new vector<Profile>){

}

ConfigFileParser::~ConfigFileParser(){
  free(mpProfiles);
}

/**
 * @todo parser should use one grammar
 **/
void ConfigFileParser::parseConfigFile(string configFileName){
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  using ascii::space;

  dbg_printc(LOG_INFO, "ConfigFileParser", "ParseConfigFile", "Parse config file: %s", configFileName.c_str());
  string line;
  ifstream config_file_stream;
  config_file_stream.open(configFileName.c_str());

  if(config_file_stream.is_open())
  {
    while(config_file_stream.good()){
      getline (config_file_stream,line);

      // Parse profile name
      //@todo profile should also be closed by "]"
      //      tried to implement "]" but seems not to work
      //      for some reason.
      if(qi::phrase_parse(line.begin(),line.end()
			  ,"[" >> (*qi::char_("a-zA-Z"))[boost::bind(&ConfigFileParser::createProfile, this, _1)]
			  ,space))
	{
	  continue;

	}
      // Parse sync type
      if(qi::phrase_parse(line.begin(), line.end()
			  ,  qi::string("syncType=") >> qi::string("syncronize")[boost::bind(&ConfigFileParser::setSyncType, this, _1)] 
			  || qi::string("syncType=") >> qi::string("backup")[boost::bind(&ConfigFileParser::setSyncType, this, _1)] 
			  || qi::string("syncType=") >> qi::string("update")[boost::bind(&ConfigFileParser::setSyncType, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse sync folder
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("syncFolder=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::setSyncFolder, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse destination folder
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destFolder=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::setDestFolder, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse destination location
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destType=") >> qi::string("local")[boost::bind(&ConfigFileParser::setDestType, this, _1)] 
			  | qi::string("destType=") >> qi::string("remote")[boost::bind(&ConfigFileParser::setDestType, this, _1)] 
			  ,space))
	{
	  continue;
	  
	}
      // Parse mount Options
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destProtocol=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::setDestProtocol, this, _1)]
			  ,space))
	{
	  continue;
	  
	}

    }
    config_file_stream.close();

  }
  else 
    dbg_printc(LOG_ERR, "ConfigFileParser", "ParseConfigFile", "\nC Unable to open configfile: %s", configFileName.c_str()); 
  config_file_stream.close();
}

void ConfigFileParser::createProfile (vector<char> name){
  Profile p;
  p.SetName(name);
  mpProfiles->push_back(p);

}

void ConfigFileParser::setSyncType (string syncType){
  mpProfiles->back().SetSyncType(syncType);

}

void ConfigFileParser::setSyncFolder (vector<char> syncFolder){
  mpProfiles->back().SetSyncFolder(syncFolder);

}

void ConfigFileParser::setDestFolder (vector<char> destFolder){
  mpProfiles->back().SetDestFolder(destFolder);

}

void ConfigFileParser::setDestType(string destLocation){
  mpProfiles->back().SetDestType(destLocation);

}

void ConfigFileParser::setDestProtocol(vector<char> mountOptions){
  mpProfiles->back().SetDestProtocol(mountOptions);
}

vector<Profile> *ConfigFileParser::getProfiles(){
  return mpProfiles;
}



