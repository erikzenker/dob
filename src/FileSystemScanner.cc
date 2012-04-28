#include "FileSystemScanner.h"

FileSystemScanner::FileSystemScanner(string scanFolder, EventManager* pEventManager){
  assert(opendir(scanFolder.c_str()));
  mScanFolder = scanFolder;
  mpEventManager = pEventManager;
}

FileSystemScanner::~FileSystemScanner(){

}

string FileSystemScanner::GetScanFolder(){
  return mScanFolder;
}
