/** 
 * @file      FileIndex.h
 * @author    Erik Zenker
 * @date      11.03.2013
 * @copyright Gnu Public License
 **/
#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <string>
#include <dirent.h>   // DIR, dirent
#include <sys/stat.h> // lstat64
#include <cstring>    // strcmp
#include <errno.h>    // errno
#include <mongo/client/dbclient.h>
#include <dbg_print.h>


class FileIndex {
public :
  FileIndex(std::string rootPath);
  ~FileIndex();
  bool indexRecursively(std::string path);
  bool indexFile(std::string path);

private :
  bool isDir(std::string path);
  uint getFileSize(std::string path);
  mongo::DBClientConnection* dbClientConnection;
  std::string mRootPath;

};

#endif /* FILEINDEX_H */
