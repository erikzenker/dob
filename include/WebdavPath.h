#ifndef WebdavPath_H
#define WebdavPath_H

#include <string>

class WebdavPath {
public:
  WebdavPath(std::string host, std::string path, std::string ressourceType, std::string lastModified, std::string contentType);
  bool isFile();
  bool isDirectory();
  std::string getPath();
private:
  std::string mHost;
  std::string mPath;
  std::string mRessourceType;
  std::string mLastModified;
  std::string mContentType;

};

#endif /* WebdavPath */
