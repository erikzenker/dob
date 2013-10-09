#ifndef FileStateDatabase_H
#define FileStateDatabase_H

#include <string>
#include <sqlite3.h>
#include <dbg_print.h>
#include <boost/filesystem.hpp>
#include <map>
#include <utility>

struct FileState {
  std::string path;
  unsigned modtime;
  unsigned inode;
  bool is_dir;

};

enum ModState {FS_DELETE, FS_MODIFY, FS_CREATE};

class FileStateDatabase {
public:
  FileStateDatabase(std::string dataBaseName);
  ~FileStateDatabase();
  bool executeModState(ModState modState, FileState fileState);
  bool insertFileState(FileState fileState);
  bool updateFileState(FileState fileState);
  bool deleteFileState(FileState fileState);
  std::vector<std::pair<FileState, ModState> > updatedb(std::string rootDirectory);
  bool resetdb();
private:
  bool executeQuery(std::string query, int (*callback)(void*,int,char**,char**) ,void* userData);
  FileState createFileState(boost::filesystem::path path);
  std::map<int, FileState> createFileStateCache();

  // sqlite3_exec callbacks
  static int printQueryResult(void *NotUsed, int argc, char **argv, char **azColName);
  static int getFileStates(void *fileStates, int argc, char **argv, char **azColName);
  
  sqlite3 *mDataBase;

  };

#endif /* FileStateDatabase_H */
