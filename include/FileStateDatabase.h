#ifndef FileStateDatabase_H
#define FileStateDatabase_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include <dbg_print.h>
#include <boost/filesystem.hpp>
#include <sstream> /* std::stringstream */
#include <sys/stat.h> /* stat, lstat */
#include <map>

struct FileState {
  std::string path;
  unsigned modtime;
  unsigned inode;

};

class FileStateDatabase {
public:
  FileStateDatabase(std::string dataBaseName);
  ~FileStateDatabase();
  bool updateState(std::string rootDirectory);
private:
  bool executeQuery(std::string query, int (*callback)(void*,int,char**,char**) ,void* userData);
  bool insertFileState(FileState fileState);
  bool updateFileState(FileState fileState);
  bool resetFileState();
  FileState createFileState(boost::filesystem::path path);
  std::map<int, FileState> createFileStateCache();
  
  static int printQueryResult(void *NotUsed, int argc, char **argv, char **azColName);
  static int getFileStates(void *fileStates, int argc, char **argv, char **azColName);
  
  sqlite3 *mDataBase;

  };

#endif /* FileStateDatabase_H */
