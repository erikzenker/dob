#include "SyncManager.h"


SyncManager::SyncManager(string destFolder){
  assert(opendir(destFolder.c_str()));
  mDestFolder = destFolder;

}

SyncManager::SyncManager(){

}

void SyncManager::SyncSourceFolder(string sourceFolder){
  cerr << "\nC Syncronise Source and Destination Folder\n";
  string rsync_query = "rsync -vzru --copy-links ";
  rsync_query
    .append(sourceFolder)
    .append(" ")
    .append(mDestFolder);
  
  system(rsync_query.c_str());


}
