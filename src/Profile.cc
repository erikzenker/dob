#include "Profile.h"

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

