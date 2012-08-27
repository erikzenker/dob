/*
 * ROADMAP
 *
 * * Copy mechanism (Rsync) --> done
 * * Mounting of server data --> done with /script/mount_server but needs more work
 * * Commandline installer
 * * Scanning of server data
 * * Ask in case of big data
 * * Graphical user interface
 * * Versioning of backup data (Git)
 *
 */


// Include Libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <gtkmm.h>
#include <signal.h>

// Include Classes
#include <InotifyFileSystemScanner.h>
#include <RemoteSyncManager.h>
#include <OptimizedEventManager.h>
#include <ConfigFileParser.h>
#include <CommandLineParser.h>
#include <Tray.h>

using namespace std;

int gtkmm_test(int argc, char *argv[]){
  /*Gtk::Main kit(argc, argv);
  Tray window;
  Gtk::Main::run(window); //Shows the window and returns when it is closed.
  */

  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui/tray.glade");
  Gtk::Window * pMainWindow = 0;
  builder->get_widget("mainWindow", pMainWindow);
  app->run(*pMainWindow);
  
  return 0;

}


int main(int argc, char *argv[]){
  // Variable Definitions
  void* no_arg = NULL;
  string scanFolder;
  string destFolder;
  string configFileName;
  ConfigFileParser configFileParser;
  CommandLineParser commandLineParser;

  //return gtkmm_test(argc, argv);

  // Parser
  if(!commandLineParser.parseCommandLine(argc, argv)){
    cout << "\nC No parameters found";
    cout << "\nC Usage: ./odb --config=CONFIGFILE";
    return 0;
  }
  configFileName = commandLineParser.getConfigFileName();
  configFileParser.addKeyWord("syncFolder");
  configFileParser.addKeyWord("destFolder");
  configFileParser.parseConfigFile(configFileName);
  scanFolder = configFileParser.getValue("syncFolder");
  destFolder = configFileParser.getValue("destFolder");
  
  // Programm
  SyncManager * pSyncManager  = new RemoteSyncManager(destFolder);
  EventManager * pEventManager = new OptimizedEventManager(pSyncManager);
  InotifyFileSystemScanner inotifyScanner(scanFolder, pEventManager);
  pSyncManager->SyncSourceFolder(scanFolder);
  inotifyScanner.Start(no_arg);

  while(1){
  
  }
  
  return 0;
}
