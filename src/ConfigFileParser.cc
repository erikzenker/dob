#include <ConfigFileParser.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
// phrase_parse includes
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <Profile.h>
#include <SyncType.h>
#include <dbg_print.h>

ConfigFileParser::ConfigFileParser(){

}

ConfigFileParser::~ConfigFileParser(){

}

void checkProfileData(ProfileData profileData){
	if(!profileData.profileName.compare("")){
		throw std::runtime_error("No profile in config file or wrong syntax!");
	}
	if(!profileData.syncPath.compare("")){
		throw std::runtime_error("No syncPath in config file");
	}
	if(!profileData.destPath.compare("")){
		throw std::runtime_error("No destPath in config file");
	}

}

/**
 * @brief Parses key/value pairs from configfile with help of boost::spirit
 *
 * @param configFileName
 **/
std::vector<ProfileData> ConfigFileParser::parseConfigFile(boost::filesystem::path configFileName){
  	namespace qi = boost::spirit::qi;

	ProfileData* profileData;
	std::vector<ProfileData*> profileDatas;

  	dbg_printc(LOG_INFO, "ConfigFileParser", "ParseConfigFile", "Parse config file: %s", configFileName.c_str());
  	std::ifstream ConfigFileStream;
  	ConfigFileStream.open(configFileName.string());

  	if(ConfigFileStream.is_open()){
    		while(ConfigFileStream.good()){
  			std::string syncType;
			std::string profileName;
			std::string ignoredPath;

			std::string line;
      			getline (ConfigFileStream, line);
			std::string::iterator begin = line.begin();
			std::string::iterator end   = line.end();

			// Parse
			if(qi::parse(begin, end, "[" >> +qi::char_("a-zA-Z_0-9") >> "]", profileName)) {
				profileData = new ProfileData;
				profileDatas.push_back(profileData);
				profileData->profileName = profileName;
				continue;	
			}
			
			if(qi::parse(begin, end, "syncType="     >> qi::char_("a-z")    , syncType)){
				if(!syncType.compare("backup")) profileData->syncType = SyncType::BACKUP;
				if(!syncType.compare("update")) profileData->syncType = SyncType::UPDATE;
				if(!syncType.compare("syncronize")) profileData->syncType = SyncType::SYNCRONIZE;
			}
			if(qi::parse(begin, end, "syncPath="     >> +qi::char_ , profileData->syncPath)) continue;
			if(qi::parse(begin, end, "syncProtocol=" >> +qi::char_ , profileData->syncProtocol)) continue;
			if(qi::parse(begin, end, "destPath="     >> +qi::char_ , profileData->destPath)) continue;
			if(qi::parse(begin, end, "destHost="     >> +qi::char_ , profileData->destHost)) continue;
			if(qi::parse(begin, end, "destUser="     >> +qi::char_ , profileData->destUser)) continue;
			if(qi::parse(begin, end, "destPort="     >> +qi::char_("0-9") , profileData->destPort)) continue;
			if(qi::parse(begin, end, "destPass="     >> +qi::char_ , profileData->destPass)) continue;
			if(qi::parse(begin, end, "ignore="       >> +qi::char_ , ignoredPath)){
				profileData->ignoredPaths.push_back(ignoredPath);
				continue;
			}
			

		}


  	}
  	else {
		throw std::runtime_error("Unable to open configfile " + configFileName.string());
	}
  	ConfigFileStream.close();

	// Remove pointer
	std::vector<ProfileData> tmpProfileDatas;
	for(ProfileData* p: profileDatas){
		checkProfileData(*p);
		tmpProfileDatas.push_back(*p);
	}

	return tmpProfileDatas;
	
}




