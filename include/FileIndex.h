/** 
 * @file      FileIndex.h
 * @author    Erik Zenker
 * @date      11.03.2013
 * @copyright Gnu Public License
 **/
#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <string>     /* string */
#include <vector>     /* vector */
#include <errno.h>    /* errno */
#include <mongo/client/dbclient.h>
#include <boost/filesystem.hpp> /* filesystem, is_directory, file_size, last_write_time */
#include <dbg_print.h>


class FileIndex {
public :
  FileIndex(std::string rootPath);
  FileIndex();
  ~FileIndex();
  bool indexRecursively(std::string path, std::vector<std::string>* filesToUpdate);
  bool indexFile(boost::filesystem::path path, std::vector<std::string>* filesToUpdate);
  void setRootPath(std::string rootPath);

private :
  mongo::DBClientConnection* dbClientConnection;
  std::string mRootPath;

};

#endif /* FILEINDEX_H */
