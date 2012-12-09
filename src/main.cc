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
 * * Write destructors for all classes
 * * Give it some nice name
 * * Restart scanning after suspend
 * * Documentation for new classes
 * * Commandline installer/helper
 * * Scanning of server data
 * * Ask in case of big data
 * * Graphical user interface/wizzard
 * * Versioning of backup data (Git)
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
#include <Tray.h>
#include <ProfileFactory.h>
#include <Inotify.h>
#include <InterProcessCommunication.h>
#include <ProfileManager.h>
#include <dbg_print.h>


using namespace std;

int start_gui(int argc, char *argv[], vector<Profile> *pProfiles);
int dbg_print_level;

int main(int argc, char *argv[]){
  // Variable definitions
  vector<Profile>* pProfiles;
  string configFileName;
  ConfigFileParser configFileParser;
  CommandLineParser commandLineParser;
  ProfileFactory profileFactory;
  FileSystemScanner *fileSystemScanner;
  InterProcessCommunication ipc("/tmp/odb_fifo");
  ProfileManager *pProfileManager;
  bool noGui;
  
  dbg_print_level = LOG_DBG;
  
  // Parse commandline
  dbg_print(LOG_INFO, "", "main","Start opendropbox client");
  if(!commandLineParser.ParseCommandLine(argc, argv)){
    dbg_printc(LOG_ERR,"Main", "main", "No commandline parameters found");
    dbg_printc(LOG_ERR,"Main", "main", "Usage: ./odb --config=CONFIGFILE [-d=DEBUG_LEVEL] [--nogui]\n");
    return 0;
  }
  noGui = commandLineParser.GetNoGui();
  dbg_print_level = commandLineParser.GetDebugLevel();

  // Parse configfile
  configFileName = commandLineParser.GetConfigFileName();
  configFileParser.ParseConfigFile(configFileName);
  pProfiles = configFileParser.GetProfiles();

  // Setup Profiles
  if(!profileFactory.MakeProfiles(pProfiles)){
    dbg_printc(LOG_FATAL, "Main","main", "Profile(s) can´t be generated from this profile, please check it\n");
    return 0;
  }

  // Setup ProfileManager
  pProfileManager = new ProfileManager(pProfiles);
  ipc.GetStopSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::StopProfile));
  ipc.GetStartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::StartProfile));
  ipc.GetRestartSignal().connect(sigc::mem_fun(*pProfileManager, &ProfileManager::RestartProfile));
 
  if(noGui){
    // Start sync without gui
    vector<Profile>::iterator profileIter;
    for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
      profileIter->StartProfile();

    }
    ipc.Read();    

  }
  else{
    // Start sync with gui
    return start_gui(argc, argv, pProfiles);
  }
  return 0;
  
}

int start_gui(int argc, char *argv[], vector<Profile>* pProfiles){
  Gtk::Main kit(argc, argv);
  Tray *tray = new Tray(pProfiles);
  
  vector<Profile>::iterator profileIter;
  for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
      profileIter->GetEventManager()->SignalEvent().connect(sigc::mem_fun(*tray,
              &Tray::OnEventManagerSignal) );

  }

  Gtk::Main::run(*tray);
  return 0;

}
