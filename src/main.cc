/*
 * ROADMAP
 *
 * 1. Copy mechanism (Rsync) --> done
 * 2. Versioning of backup data (Git)
 * 3. Mounting of server data
 * 4. Scanning of server data
 * 6. Ask in case of big data
 * 5. Graphical user interface
 *
 */


// Include Libraries
#include <iostream>
#include <string>
#include <stdlib.h>

// Include Classes
#include <InotifyFileSystemScanner.h>
#include <RemoteSyncManager.h>
#include <OptimizedEventManager.h>
#include <ConfigFileParser.h>
#include <CommandLineParser.h>


using namespace std;

int main(int argc, char *argv[]){
  // Variable Definitions
  void* no_arg = NULL;
  string scanFolder;
  string destFolder;
  string configFileName;
  ConfigFileParser configFileParser;
  CommandLineParser commandLineParser;

  // Parser
  commandLineParser.parseCommandLine(argc, argv);
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
    system("sleep 1s");
  }

  return 0;
}
