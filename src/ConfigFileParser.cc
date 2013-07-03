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
      // Parse sync protocol
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("syncProtocol=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::setSyncProtocol, this, _1)]
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
      // Parse destination host
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destHost=") 
			  >> 
			  (*qi::char_)[boost::bind(&ConfigFileParser::setDestHost, this, _1)] 
			  ,space))
	{
	  continue;
	  
	}
      // Parse destination user
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destUser=") 
			  >> 
			  (*qi::char_)[boost::bind(&ConfigFileParser::setDestUser, this, _1)] 
			  ,space))
	{
	  continue;
	  
	}
      // Parse ssh port
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("sshPort=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::setSshPort, this, _1)]
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

void ConfigFileParser::setSyncProtocol(std::vector<char> syncProto){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setSyncProtocol(syncProto);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setSyncProtocol", "Try to set syncProtocol, but there is no profile"); 
}


void ConfigFileParser::setDestFolder( std::vector<char> destFolder){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setDestFolder(destFolder);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestFolder", "Try to set destFolder, but there is no profile"); 

}

void ConfigFileParser::setDestHost(std::vector<char> destHost){
  if(mpProfiles->size() != 0) {
    std::string toString( destHost.begin(), destHost.end() );
    mpProfiles->back().setDestHost(toString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestHost", "Try to set destHost, but there is no profile"); 

}
/*
void ConfigFileParser::setDestUser(std::string destUser){
  if(mpProfiles->size() != 0)
    mpProfiles->back().setDestUser(destUser);
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestUser", "Try to set destUser, but there is no profile"); 

}
*/
void ConfigFileParser::setDestUser(std::vector<char> destUser){
  if(mpProfiles->size() != 0){
    std::string toString( destUser.begin(), destUser.end() );
    mpProfiles->back().setDestUser(toString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestUser", "Try to set destUser, but there is no profile"); 

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
     mpProfiles->back().setDestPort(portString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setDestPort", "Try to set destination port, but there is no profile"); 
  
}

void ConfigFileParser::setSshPort(std::vector<char> sshPort){
  if(mpProfiles->size() != 0){
    std::string portString(sshPort.begin(), sshPort.end());
     mpProfiles->back().setSshPort(portString);
  }
  else
    dbg_printc(LOG_WARN, "ConfigFileParser", "setSshPort", "Try to set SSH port, but there is no profile"); 
  
}


/**
 * @return all parsed profiles
 **/
vector<Profile> *ConfigFileParser::getProfiles(){
  return mpProfiles;
}



