#include "Profile.h"

Profile::Profile() : 
  name(""),
  syncType(""),
  syncFolder(""),
  destFolder(""),
  mDestType(""),
  mDestProtocol(""){

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
  cout << "\nC  destType=" << mDestType;
  cout << "\nC  destProtocol=" << mDestProtocol;
  cout << "\n";
  
}

bool Profile::StartProfile(){
  return StartSyncProfile() && StartScanProfile();
}

bool Profile::StartSyncProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start sync with profile: [\033[32m%s\033[m] ", name.c_str());
  return mpSyncManager->SyncSourceFolder(mpFileSystemScanner->GetScanFolder());
}

bool Profile::StartScanProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->StartToScan();
  return true;
}
bool Profile::StopProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Stop scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->StopToScan();
  return true;
}

