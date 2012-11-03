#include "ConfigFileParser.h"
ConfigFileParser::ConfigFileParser():
  mpProfiles(new vector<Profile>){

}

/**
 * @todo parser should use one grammar
 **/
void ConfigFileParser::ParseConfigFile(string configFileName){
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  //namespace phoenix = boost::phoenix;

  //using qi::char_;
  using ascii::space;

  dbg_print(LOG_INFO, "\nC Parse config file: %s", configFileName.c_str());
  string line;
  ifstream config_file_stream;
  config_file_stream.open(configFileName.c_str());

  if(config_file_stream.is_open())
  {
    while(config_file_stream.good())
    {
      getline (config_file_stream,line);

      // Parse profile name
      //@todo profile should also be closed by "]"
      //      tried to implement "]" but seems not to work
      //      for some reason.
      if(qi::phrase_parse(line.begin(),line.end()
			  ,"[" >> (*qi::char_("a-zA-Z"))[boost::bind(&ConfigFileParser::CreateProfile, this, _1)]
			  ,space))
	{
	  continue;

	}
      // Parse sync type
      if(qi::phrase_parse(line.begin(), line.end()
			  ,  qi::string("syncType=") >> qi::string("syncronize")[boost::bind(&ConfigFileParser::SetSyncType, this, _1)] 
			  || qi::string("syncType=") >> qi::string("backup")[boost::bind(&ConfigFileParser::SetSyncType, this, _1)] 
			  || qi::string("syncType=") >> qi::string("update")[boost::bind(&ConfigFileParser::SetSyncType, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse sync folder
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("syncFolder=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::SetSyncFolder, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse destination folder
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destFolder=") 
			  >>     
			  (*qi::char_)[boost::bind(&ConfigFileParser::SetDestFolder, this, _1)]
			  ,space))
	{
	  continue;
	  
	}
      // Parse destination location
      if(qi::phrase_parse(line.begin(), line.end()
			  , qi::string("destLocation=") >> qi::string("local")[boost::bind(&ConfigFileParser::SetDestLocation, this, _1)] 
			  | qi::string("destLocation=") >> qi::string("remote")[boost::bind(&ConfigFileParser::SetDestLocation, this, _1)] 
			  ,space))
	{
	  continue;
	  
	}

    }
    config_file_stream.close();

  }
  else 
    dbg_print(LOG_ERR, "\nC Unable to open configfile: %s", configFileName.c_str()); 
  config_file_stream.close();
}

void ConfigFileParser::CreateProfile (vector<char> name){
  Profile p;
  p.SetName(name);
  mpProfiles->push_back(p);

}

void ConfigFileParser::SetSyncType (string syncType){
  mpProfiles->back().SetSyncType(syncType);

}

void ConfigFileParser::SetSyncFolder (vector<char> syncFolder){
  mpProfiles->back().SetSyncFolder(syncFolder);

}

void ConfigFileParser::SetDestFolder (vector<char> destFolder){
  mpProfiles->back().SetDestFolder(destFolder);

}

void ConfigFileParser::SetDestLocation (string destLocation){
  mpProfiles->back().SetDestLocation(destLocation);

}

vector<Profile> *ConfigFileParser::GetProfiles(){
  return mpProfiles;
}

