// Include Libraries
#include <iostream>
#include <string>

// Include Classes
#include <InotifySystemScanner.h>

using namespace std;

int main(){
  void* no_arg;
  string scan_folder = "/home/erik/OpenDropBox";
  string dest_folder = "/home/erik/OpenDropBoxServer";
  InotifyFileSystemScanner iscanner(scan_folder);
  iscanner.Start(no_arg);

  while(1){
    sleep(1000);
  }
  
  return 0;
}
