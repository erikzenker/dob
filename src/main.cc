/***
 * ROADMAP
 * *
 * * Copy mechanism (Rsync)                                                 --> done
 * * Mounting of server data                                                --> done : by server installation in vm
 * * Debug levels                                                           --> done : by dbg_print.h 
 * * Also non X userinterface (console only)	
 * * Write destructors for all classes
 * * Give it some nice name
 * * Create InotifyFileSystemScanner alternative because is not threadsafe
 * * Replace libinotify with own implementation
 * * Write Wrapper for Inotify events for more general use
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

  // Parse commandline and configfile
  if(!commandLineParser.ParseCommandLine(argc, argv)){
    dbg_print(LOG_INFO, "\nC No commandline parameters found");
    dbg_print(LOG_INFO, "\nC Usage: ./odb --config=CONFIGFILE [-d=DEBUG_LEVEL]\n");
    return 0;

  }
  dbg_print_level = commandLineParser.GetDebugLevel();
  configFileName = commandLineParser.GetConfigFileName();
  configFileParser.ParseConfigFile(configFileName);
  pProfiles = configFileParser.GetProfiles();

  // Make profiles (instanciate necessari sync objects)
  if(!profileFactory.MakeProfiles(pProfiles)){
    dbg_print(LOG_FATAL,"\nC Profiles can´t be generated from this profile\n");
    return 0;
  }
  
  // Until inotify is threadsafe just one profile can be handled
  if(pProfiles->size() > 1){
    dbg_print(LOG_FATAL,"\nC Can´ t handle more then one profile, please comment all except of one\n");
    return 0;
  }

  /*
  dbg_print(LOG_DBG,"\nC Start to Watch folder");
  Inotify inotify;
  inotify.WatchFolderRecursively(pProfiles->at(0).GetSyncFolder());
  inotify.GetNextEvent();

  while(1){
    inotify.GetNextEvent();
  }
  */

  // Start experimental gui
  return start_gui(argc, argv, pProfiles);
  //return 0;
  
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

  /*
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Gtk::Window window;
  Glib::RefPtr<Gtk::StatusIcon> m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);
  return app->run(window);
  */

}
