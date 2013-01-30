#include "FileSystemScanner.h"

FileSystemScanner::FileSystemScanner(string scanFolder, EventManager* const pEventManager):
mScanFolder(scanFolder),
mpEventManager(pEventManager){
  assert(opendir(scanFolder.c_str()));
}

FileSystemScanner::~FileSystemScanner(){
  free(mpEventManager);
}

string FileSystemScanner::getScanFolder() const{
  return mScanFolder;

}

EventManager* FileSystemScanner::getEventManager() const{
  return mpEventManager;

}



