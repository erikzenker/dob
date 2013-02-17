#include "ConfigFileParser.h"
ConfigFileParser::ConfigFileParser():
  mpProfiles(new vector<Profile>){

}

ConfigFileParser::~ConfigFileParser(){
  free(mpProfiles);
}

/**
 * @brief Parses key/value pairs from configfile with help of boost::spirit
 *
 * @todo  parser should use one grammar and should be prettier
 * @param location of the configfile
 **/
void ConfigFileParser::parseConfigFile(std::string configFileName){
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  using ascii::space;

  dbg_printc(LOG_INFO, "ConfigFileParser", "ParseConfigFile", "Parse config file: %s", configFileName.c_str());
  std::string line;
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

      // Parse ignored files / folders
      if(qi::phrase_parse(line.begin(), line.end()
      			  , qi::string("ignore=") 
      			  >>     
      			  (*qi::char_)[boost::bind(&ConfigFileParser::pushIgnoredFolder, this, _1)]
      			  ,space))
      	{
      	  continue;
	  
      	}
      // Parse destPort
      if(qi::phrase_parse(line.begin(), line.end()
      			  , qi::string("destPort=") 
      			  >>     
      			  (*qi::char_)[boost::bind(&ConfigFileParser::setDestPort, this, _1)]
      			  ,space))
      	{
      	  continue;
	  
      	}



    }
    config_file_stream.close();

  }
  else 
    dbg_printc(LOG_ERR, "ConfigFileParser", "ParseConfigFile", "Unable to open configfile: %s", configFileName.c_str()); 
  config_file_stream.close();
}

void ConfigFileParser::createProfile (vector<char> name){
  Profile profile;
  profile.setName(name);
  mpProfiles->push_back(profile);

}

void ConfigFileParser::setSyncType (std::string syncType){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setSyncType(syncType);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setSyncType", "Try to set syncType, but there is no profile"); 

}

void ConfigFileParser::setSyncFolder (vector<char> syncFolder){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setSyncFolder(syncFolder);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setSyncFolder", "Try to set syncFolder, but there is no profile"); 

}

void ConfigFileParser::setDestFolder (vector<char> destFolder){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setDestFolder(destFolder);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestFolder", "Try to set destFolder, but there is no profile"); 

}

void ConfigFileParser::setDestType(std::string destLocation){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setDestType(destLocation);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestLocation", "Try to set destLocation, but there is no profile"); 

}

void ConfigFileParser::setDestProtocol(std::vector<char> destProtocol){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setDestProtocol(destProtocol);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestProtocol", "Try to set destProtocol, but there is no profile"); 
}

void ConfigFileParser::pushIgnoredFolder(std::vector<char> ignoredFolder){
  if(mpProfiles->size() != 0){
    std::string toString(ignoredFolder.begin(), ignoredFolder.end());
    mpProfiles->back().pushIgnoredFolder(toString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "pushIgnoredFolder", "Try to push ignored folder, but there is no profile"); 

}

void ConfigFileParser::setDestPort(std::vector<char> port_char){
  if(mpProfiles->size() != 0){
    std::string portString(port_char.begin(), port_char.end());
    dbg_printc(LOG_DBG, "ConfigFileParser", "setDestPort", "destport %s", portString.c_str()); 
    mpProfiles->back().setDestPort(portString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestPort", "Try to set destination port, but there is no profile"); 
  
}

/**
 * @return all parsed profiles
 **/
vector<Profile> *ConfigFileParser::getProfiles(){
  return mpProfiles;
}



