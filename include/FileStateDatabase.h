#ifndef FileStateDatabase_H
#define FileStateDatabase_H

#include <string>
#include <sqlite3.h>
#include <map>
#include <utility> /* pair */
#include <vector>
#include <boost/filesystem.hpp>
#include <FileState.h>



enum ModState {FS_DELETE, FS_MODIFY, FS_CREATE};

class FileStateDatabase {
public:
  FileStateDatabase(std::string dataBaseName, boost::filesystem::path rootPath);
  ~FileStateDatabase();

  // methods
  std::vector<std::pair<FileState, ModState> > updates();
  bool propagateUpdate(const std::pair<FileState, ModState> update, const bool recursive);
  bool propagateUpdate(const boost::filesystem::path path, const ModState ms, const bool recursive);

  FileState getFileState(const boost::filesystem::path path);

  // statics
  static std::string modStateToString(const ModState modState);

private:
  // Modify database
  bool insertFileState(const FileState fileState);
  bool updateFileState(const FileState fileState);
  bool deleteFileState(const FileState fileState, const bool recursive);
  void setFileState(const FileState fs);
  bool executeQuery(const std::string query, int (*callback)(void*,int,char**,char**) ,void* userData);

  FileState createFileState(boost::filesystem::path path);
  void createFileStateCache();

  // sqlite3_exec callbacks
  static int noAction(void *NotUsed, int argc, char **argv, char **azColName);
  static int getFileStates(void *fileStates, int argc, char **argv, char **azColName);
  
  // Members
  sqlite3 *mDataBase;
  boost::filesystem::path mRootPath;
  std::map<std::string, FileState> mFileStateCache;

};

#endif /* FileStateDatabase_H */
