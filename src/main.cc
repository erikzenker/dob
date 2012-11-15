/***
 * ROADMAP
 * *
 * * Copy mechanism (Rsync)                                  --> done
 * * Mounting of server data                                 --> done : by server installation in vm
 * * Debug levels                                            --> done : by dbg_print.h 
 * * Also non X userinterface (console only)                 --> done : by InterProcessCommunication interface	
 * * Write destructors for all classes
 * * Give it some nice name
 * * Replace libinotify with own implementation             --> done : by Inotify.h but not all functions
 * * Write Wrapper for Inotify events for more general use  --> done : FileSystemEvent.h but is kind of ugly
 * * Write some nice interface for ipc
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
  InterProcessCommunication *ipc = new InterProcessCommunication("/home/erik/projects/open_drop_box/fifo");
  bool noGui;
  
  dbg_print_level = LOG_DBG;

  dbg_print(LOG_INFO, "", "main","Start opendropbox client");
  // Parse commandline and configfile
  if(!commandLineParser.ParseCommandLine(argc, argv)){
    dbg_printc(LOG_ERR,"Main", "main", "No commandline parameters found");
    dbg_printc(LOG_ERR,"Main", "main", "Usage: ./odb --config=CONFIGFILE [-d=DEBUG_LEVEL] [--nogui]\n");
    return 0;

  }
  noGui = commandLineParser.GetNoGui();
  dbg_print_level = commandLineParser.GetDebugLevel();
  configFileName = commandLineParser.GetConfigFileName();
  configFileParser.ParseConfigFile(configFileName);
  pProfiles = configFileParser.GetProfiles();

  // Make profiles (instanciate necessary sync objects)
  if(!profileFactory.MakeProfiles(pProfiles)){
    dbg_printc(LOG_FATAL, "Main","main", "Profile(s) can´t be generated from this profile, please check it\n");
    return 0;
  }
  // Start sync without gui
  if(noGui){
    vector<Profile>::iterator profileIter;
    for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
      dbg_printc(LOG_INFO, "Main", "main", "Start sync with profile: [\033[32m%s\033[m] ", profileIter->GetName().c_str());
      profileIter->GetSyncManager()->SyncSourceFolder(profileIter->GetFileSystemScanner()->GetScanFolder());

    }

    //@bug cant start scanning directly after sync different profiles
    FileSystemScanner *fileSystemScanner;
    for(profileIter = pProfiles->begin(); profileIter < pProfiles->end(); profileIter++){
      dbg_printc(LOG_INFO, "Main", "main", "Start scanning with profile: [\033[32m%s\033[m] ", profileIter->GetName().c_str());
      fileSystemScanner = profileIter->GetFileSystemScanner();
      profileIter->GetFileSystemScanner() ->StartToScan();
      ipc->GetStopSignal().connect(sigc::mem_fun(*fileSystemScanner, &FileSystemScanner::StopToScan));
      ipc->GetStartSignal().connect(sigc::mem_fun(*fileSystemScanner, &FileSystemScanner::StartToScan));

    }
    ipc->Read();    
    while(1);
  }
  else{
    // Start sync with experimental gui
    return start_gui(argc, argv, pProfiles);
  }
  return 0;
  
}

int start_gui(int argc, char *argv[], vector<Profile>* pProfiles){
  /*
 Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui/tray.glade");
  Gtk::Window * pMainWindow = 0;
  builder->get_widget("mainWindow", pMainWindow);
  app->run(*pMainWindow);
  */

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
