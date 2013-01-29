/***
 * ROADMAP
 * *
 * * Copy mechanism (Rsync)                                  --> done
 * * Mounting of server data                                 --> done : by server installation in vm and scripts/
 * * Debug levels                                            --> done : by dbg_print.h 
 * * Also non X userinterface (console only)                 --> done : by InterProcessCommunication interface	
 * * Replace libinotify with own implementation              --> done : by Inotify.h but not all functions
 * * Write Wrapper for Inotify events for more general use   --> done : FileSystemEvent.h but is kind of ugly
 * * Write some nice interface for ipc                       --> done : echo start default > /tmp/odb_fifo
 * * Give it some nice name                                  --> done : dob
 * * Remove all warnings from compiling
 * * Rename all methods to small camelcase
 * * Versioning of backup data (Git/Database)
 * * Write destructors for all classes
 * * Restart scanning after suspend
 * * Documentation for new classes
 * * Commandline installer/helper
 * * Scanning of server data
 * * Ask in case of big data
 * * Graphical user interface/wizzard
 * *
 ***/

// Include libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <gtkmm.h>
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
  InterProcessCommunication ipc("/tmp/odb_fifo");
  ProfileManager *pProfileManager;
  
  dbg_print_level = LOG_DBG;
  
  // Parse commandline
  dbg_print(LOG_INFO, "", "main","Start dob client");
  if(!commandLineParser.parseCommandLine(argc, argv)){
    dbg_printc(LOG_ERR,"Main", "main", "No commandline parameters found");
    dbg_printc(LOG_ERR,"Main", "main", "Usage: ./dob --config=CONFIGFILE [-d=DEBUG_LEVEL]\n");
    return 0;
  }
  useGui = commandLineParser.getUseGui(); 
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
  ipc.GetStopSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::StopProfile));
  ipc.GetStartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::StartProfile));
  ipc.GetRestartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::RestartProfile));
 
  // Start sync
  vector<Profile>::iterator profileIter;
  for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
    profileIter->StartProfile();
    
  }

  // Read from IPC Interface
  ipc.Read();    


  // Cleanup memory
  free(pProfiles);
  free(pProfileManager);
  return 0;
  
}
