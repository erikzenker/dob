#include "FileSystemScanner.h"

FileSystemScanner::FileSystemScanner(std::string scanFolder, EventManager* const pEventManager):
mScanFolder(scanFolder),
mpEventManager(pEventManager){
  assert(opendir(scanFolder.c_str()));
  if(mScanFolder[mScanFolder.size()-1] != '/')
     mScanFolder.append("/");

}

FileSystemScanner::~FileSystemScanner(){
  free(mpEventManager);
}

std::string FileSystemScanner::getScanFolder() const{
  return mScanFolder;

}

EventManager* FileSystemScanner::getEventManager() const{
  return mpEventManager;

}



