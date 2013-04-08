
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
#include <iostream>
#include <string>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Include classes
#include <InotifyFileSystemScanner.h>
#include <RemoteSyncManager.h>
#include <LocalSyncManager.h>
#include <OptimizedEventManager.h>
#include <ConfigFileParser.h>
#include <CommandLineParser.h>
#include <ProfileFactory.h>
#include <Inotify.h>
#include <InterProcessCommunication.h>
#include <ProfileManager.h>
#include <dbg_print.h>

using namespace std;

int dbg_print_level;

int main(int argc, char *argv[]){
  // Variable definitions
  vector<Profile>* pProfiles;
  string configFileName;
  ConfigFileParser configFileParser;
  CommandLineParser commandLineParser;
  ProfileFactory profileFactory;
  InterProcessCommunication ipc("/tmp/dob_fifo");
  ProfileManager *pProfileManager;
  
  dbg_print_level = LOG_DBG;
  
  // Parse commandline
  dbg_printc(LOG_INFO, "", "main","Start dob client");
  if(!commandLineParser.parseCommandLine(argc, argv)){
    dbg_printc(LOG_ERR,"Main", "main", "No commandline parameters found");
    dbg_printc(LOG_ERR,"Main", "main", "Usage: ./dob --config=CONFIGFILE [-d=DEBUG_LEVEL]\n");
    return 0;
  }
  dbg_print_level = commandLineParser.getDebugLevel();

  // Parse configfile
  configFileName = commandLineParser.getConfigFileName();
  configFileParser.parseConfigFile(configFileName);
  pProfiles = configFileParser.getProfiles();

  // Setup Profiles
  if(!profileFactory.makeProfiles(pProfiles)){
    dbg_printc(LOG_FATAL, "Main","main", "Profile(s) can´t be generated from this config file, please check it\n");
    return 0;
  }

  if(pProfiles->size() == 0){
    dbg_printc(LOG_ERR, "Main","main", "There are no profiles defined in your config file: %s\n", configFileName.c_str());
    return 0;
  }

  // Setup ProfileManager
  pProfileManager = new ProfileManager(pProfiles);
  ipc.getStopSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::stopProfile));
  ipc.getStartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::startProfile));
  ipc.getRestartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::restartProfile));
 
  // Start sync 
  // @todo ProfileManager should do this task
  vector<Profile>::iterator profileIter;
  for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
    profileIter->startProfile();
    
  }

  // Read from IPC Interface
  ipc.readFromPipe();    


  // Cleanup memory
  free(pProfiles);
  free(pProfileManager);
  return 0;
  
}
