/***
 * ROADMAP
 * *
 * * Copy mechanism (Rsync)                                  --> done
 * * Mounting of server data                                 --> done : by server installation in vm
 * * Debug levels                                            --> done : by dbg_print.h 
 * * Also non X userinterface (console only)	
 * * Write destructors for all classes
 * * Give it some nice name
 * * Replace libinotify with own implementation             --> done : by Inotify.h but not all functions
 * * Write Wrapper for Inotify events for more general use  --> done : FileSystemEvent.h
 * * Documentation for new classes
 * * Commandline installer
 * * Scanning of server data
 * * Ask in case of big data
 * * Graphical user interface
 * * Versioning of backup data (Git)
 * *
 ***/

// Include libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <gtkmm.h>
#include <signal.h>

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
  
  dbg_print_level = LOG_DBG;

  dbg_print(LOG_INFO, "", "main","Start opendropbox client");
  // Parse commandline and configfile
  if(!commandLineParser.ParseCommandLine(argc, argv)){
    dbg_printc(LOG_ERR,"", "main", "No commandline parameters found");
    dbg_printc(LOG_ERR,"", "main", "Usage: ./odb --config=CONFIGFILE [-d=DEBUG_LEVEL]\n");
    return 0;

  }
  dbg_print_level = commandLineParser.GetDebugLevel();
  configFileName = commandLineParser.GetConfigFileName();
  configFileParser.ParseConfigFile(configFileName);
  pProfiles = configFileParser.GetProfiles();

  // Make profiles (instanciate necessary sync objects)
  if(!profileFactory.MakeProfiles(pProfiles)){
    dbg_print(LOG_FATAL, "main", "Profiles can´t be generated from this profile\n");
    return 0;
  }
  
  // Start experimental gui
  return start_gui(argc, argv, pProfiles);
  
}
/**
 * @todo gui can only handle one profile should be able to handle more!
 **/
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
