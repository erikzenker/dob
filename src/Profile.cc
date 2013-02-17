#include "Profile.h"

Profile::Profile() : 
  name(""),
  mSyncType(""),
  mSyncFolder(""),
  mDestFolder(""),
  mDestType(""),
  mDestProtocol(""),
  mDestPort(""){

}

Profile::~Profile(){
  // free(mpSyncManager);
  // free(mpEventManager);
  // free(mpFileSystemScanner);
}


bool Profile::isValid(){
  if(!mSyncFolder.compare("") && !mDestFolder.compare(""))
    return false;
  return true;
}

void Profile::print(){
  cout << "\nC [" << name << "]";
  cout << "\nC  syncType=" << mSyncType;
  cout << "\nC  syncFolder=" << mSyncFolder;
  cout << "\nC  destFolder=" << mDestFolder;
  cout << "\nC  destType=" << mDestType;
  cout << "\nC  destProtocol=" << mDestProtocol;
  cout << "\nC  destPort=" << mDestPort;
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

