#include "FileSystemScanner.h"

FileSystemScanner::FileSystemScanner(string scanFolder, EventManager* const pEventManager):
mScanFolder(scanFolder),
mpEventManager(pEventManager){
  assert(opendir(scanFolder.c_str()));
}

FileSystemScanner::~FileSystemScanner(){

}

string FileSystemScanner::GetScanFolder() const{
  return mScanFolder;
}
