#ifndef FileStateDatabase_H
#define FileStateDatabase_H

#include <string>
#include <sqlite3.h>
#include <map>
#include <utility> /* pair */
#include <vector>
#include <boost/filesystem.hpp>

struct FileState {
  boost::filesystem::path path;
  unsigned modtime;
  unsigned inode;
  bool is_dir;

};

enum ModState {FS_DELETE, FS_MODIFY, FS_CREATE};

class FileStateDatabase {
public:
  FileStateDatabase(std::string dataBaseName, boost::filesystem::path rootPath);
  ~FileStateDatabase();
  bool propagateUpdate(std::pair<FileState, ModState> update);
  bool propagateUpdate(boost::filesystem::path path, ModState ms);
  static std::string modStateToString(ModState modState);
  bool resetdb();

  std::vector<std::pair<FileState, ModState> > getUpdates();

private:
  bool insertFileState(FileState fileState);
  bool updateFileState(FileState fileState);
  bool deleteFileState(FileState fileState);
  FileState createFileState(boost::filesystem::path path);
  bool executeQuery(std::string query, int (*callback)(void*,int,char**,char**) ,void* userData);
  std::map<int, FileState> createFileStateCache();

  // sqlite3_exec callbacks
  static int noAction(void *NotUsed, int argc, char **argv, char **azColName);
  static int getFileStates(void *fileStates, int argc, char **argv, char **azColName);
  
  sqlite3 *mDataBase;
  boost::filesystem::path mRootPath;

};

#endif /* FileStateDatabase_H */
