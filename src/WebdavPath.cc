#include <string> /* compare */
#include <exception> /* exception */
#include "WebdavPath.h"
#include "dbg_print.h"

WebdavPath::WebdavPath(std::string host, std::string path, std::string ressourceType, std::string lastModified, std::string contentType) :
  mHost(host),
  mPath(path),
  mRessourceType(ressourceType),
  mLastModified(lastModified),
  mContentType(contentType){

}

bool WebdavPath::isFile(){
  if(!mRessourceType.compare(""))
    return true;
  return false;

}

bool WebdavPath::isDirectory(){
  return !isFile();

}

std::string WebdavPath::getPath(){
  return mPath;

}
