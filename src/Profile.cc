#include "Profile.h"

Profile::Profile() : 
  name(""),
  mSyncType(DOB_BACKUP),
  mSyncFolder(""),
  mSyncProto(""),
  mDestUser(""),
  mDestHost(""),
  mDestPort(""),
  mDestFolder(""),
  mSshPort("22"){
  

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
  std::cout << "\nC [" << name << "]";
  std::cout << "\nC  syncType=" << mSyncType;
  std::cout << "\nC  syncFolder=" << mSyncFolder;
  std::cout << "\nC  syncProto=" << mSyncProto;
  std::cout << "\nC  destUser=" << mDestUser;
  std::cout << "\nC  destHost=" << mDestHost;
  std::cout << "\nC  destPort=" << mDestPort;
  std::cout << "\nC  destFolder=" << mDestFolder;
  std::cout << "\nC  SshPort=" << mSshPort;
  std::cout << "\n";
  
}

bool Profile::startProfile(){
  return startSyncProfile() && startScanProfile();
}

bool Profile::startSyncProfile(){
  dbg_printc(LOG_INFO, "Main", "main", "Start sync with profile: [\033[32m%s\033[m] ", name.c_str());
  return true;
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

