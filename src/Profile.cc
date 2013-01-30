#include "Profile.h"

Profile::Profile() : 
  name(""),
  syncType(""),
  syncFolder(""),
  destFolder(""),
  mDestType(""),
  mDestProtocol(""){

}

Profile::~Profile(){
  // free(mpSyncManager);
  // free(mpEventManager);
  // free(mpFileSystemScanner);
}


bool Profile::isValid(){
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

bool Profile::startProfile(){
  return startSyncProfile() && startScanProfile();
}

bool Profile::startSyncProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start sync with profile: [\033[32m%s\033[m] ", name.c_str());
  return mpSyncManager->syncSourceFolder(mpFileSystemScanner->getScanFolder());
}

bool Profile::startScanProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->startToScan();
  return true;
}
bool Profile::stopProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Stop scanning with profile: [\033[32m%s\033[m] ", name.c_str());
  mpFileSystemScanner->stopToScan();
  return true;
}

