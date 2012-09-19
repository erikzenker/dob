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

int gtkmm_test(int argc, char *argv[], FileSystemScanner *pFileSystemScanner){
  /*
 Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui/tray.glade");
  Gtk::Window * pMainWindow = 0;
  builder->get_widget("mainWindow", pMainWindow);
  app->run(*pMainWindow);
  */

  Gtk::Main kit(argc, argv);
  Tray tray(pFileSystemScanner);
  pFileSystemScanner->GetEventManager()->GetSyncManager()->mrStatusIcon = tray.GetStatusIcon();
  pFileSystemScanner->GetEventManager()->mrStatusIcon = tray.GetStatusIcon();
  Gtk::Main::run(tray);
  return 0;

  /*
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  Gtk::Window window;
  Glib::RefPtr<Gtk::StatusIcon> m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);
  return app->run(window);
  */

}


int main(int argc, char *argv[]){
  // Variable Definitions
  void* no_arg = NULL;
  string scanFolder;
  string destFolder;
  string configFileName;
  ConfigFileParser configFileParser;
  CommandLineParser commandLineParser;

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
  FileSystemScanner * pFileSystemScanner = new InotifyFileSystemScanner(scanFolder, pEventManager);

  // GUI
  return gtkmm_test(argc, argv, pFileSystemScanner);

}
