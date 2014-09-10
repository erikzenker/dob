
 /***
 * ROADMAP
 * *
 * * Versioning of backup data (Git/Database)
 * * Restart scanning after suspend
 * * Commandline installer/helper
 * * Scanning/polling of server data periodically
 * * Ask in case of big data
 * * Graphical user interface/wizzard
 * *
 ***/

// Include libraries
#include <vector>
#include <string>

#include <LocalSyncManager.h>
#include <FileEventManager.h>
#include <InotifyFileSystemScanner.h>
#include <Profile.h>
#include <ProfileFactory.h>
#include <CommandLineParser.h>
#include <dbg_print.h>

int dbg_print_level;

int main(int argc, char *argv[]){
  	// Variable definitions
  	CommandLineParser commandLineParser;
 	ProfileFactory profileFactory;
	std::vector<Profile> profiles;
  	dbg_print_level = LOG_DBG;
  
  	// Parse commandline
  	if(!commandLineParser.parseCommandLine(argc, argv)){
   	 	// dbg_printc(LOG_ERR,"Main", "main", "No commandline parameters found");
   	 	// dbg_printc(LOG_ERR,"Main", "main", "Usage: ./dob --config=CONFIGFILE [-d=DEBUG_LEVEL]\n");
    		return 0;
  	}

	// Parse profiles from configfile
 	profiles = profileFactory.makeProfiles(commandLineParser.getConfigFileName());

	// Start profiles
 	for(Profile profile: profiles){
		profile.start();
	}

  	return 0;
  
}
