#include "FileSystemScanner.h"

FileSystemScanner::FileSystemScanner(string scan_folder){
  this->scan_folder = scan_folder;
}

FileSystemScanner::~FileSystemScanner(){

}

string FileSystemScanner::GetScanFolder(){
  return this->scan_folder;
}
