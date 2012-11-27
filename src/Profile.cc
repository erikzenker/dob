#include "Profile.h"

Profile::Profile() : 
  name(""),
  syncType(""),
  syncFolder(""),
  destFolder(""),
  destLocation(""),
  mMountOptions(""){

}


bool Profile::IsValid(){
  if(!syncFolder.compare("") && !destFolder.compare(""))
    return false;
  return true;
}

void Profile::print(){
  cout << "\nC [" << name << "]";
  cout << "\nC  syncType=" << syncType;
  cout << "\nC  syncFolder=" << syncFolder;
  cout << "\nC  destFolder=" << destFolder;
  cout << "\nC  destLocation=" << destLocation;
  cout << "\n";
  
}

bool Profile::StartProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start sync with profile: [\033[32m%s\033[m] ", name.c_str());
  mpSyncManager->SyncSourceFolder(mpFileSystemScanner->GetScanFolder());
  dbg_printc(LOG_INFO, "Main", "main", "Start scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->StartToScan();
  return true;
}

bool Profile::StopProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Stop scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->StopToScan();
  return true;
}

