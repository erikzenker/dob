#include <FileStateDatabase.h>

FileStateDatabase::FileStateDatabase(std::string dataBaseName){
  int error = sqlite3_open(dataBaseName.c_str(), &mDataBase);
  if(error){
    dbg_printc(LOG_ERR, "FileStateDatabase", "FileStateDatabase", "Can't open database: %s", sqlite3_errmsg(mDataBase));
    sqlite3_close(mDataBase);

  }

}

FileStateDatabase::~FileStateDatabase(){
  sqlite3_close(mDataBase);

}

int FileStateDatabase::printQueryResult(void *NotUsed, int argc, char **argv, char **azColName){
  for(int i=0; i<argc; ++i){
    dbg_printc(LOG_DBG, "FileStateDatabase", "getColumns", "%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  return 0;
}


int FileStateDatabase::getFileStates(void *fileStates, int argc, char **argv, char **azColName){
  FileState fileState = {"", 0};
  fileState.path = argv[0];
  fileState.modtime = atoi(argv[1]);
  fileState.inode = atoi(argv[2]);

  ((std::vector<FileState>*)fileStates)->push_back(fileState);
  return 0;
}
  
bool FileStateDatabase::executeQuery(std::string query, int (*callback)(void*,int,char**,char**) ,void* userData){
  char *errorMsg = 0;
  int error = sqlite3_exec(mDataBase, query.c_str(), callback, userData, &errorMsg);
  if(error){
    dbg_printc(LOG_ERR, "FileStateDatabase", "updateState", "SQL error: %s", errorMsg);
    sqlite3_free(errorMsg);
    return false;
  }
  return true;

}

FileState FileStateDatabase::createFileState(boost::filesystem::path path){
  FileState fileState;
  struct stat buffer;
  fileState.path    = path.string();
  fileState.modtime = boost::filesystem::last_write_time(path);
  fileState.inode   = lstat(path.c_str(), &buffer)? 0 : buffer.st_ino;

  return fileState;
}

std::map<int, FileState> FileStateDatabase::createFileStateCache(){
  std::vector<FileState> fileStates;
  std::map<int, FileState> fileStateCache;
  executeQuery("SELECT path, modtime, inode FROM statedb", FileStateDatabase::getFileStates, &fileStates);
  for(unsigned i = 0; i < fileStates.size(); ++i){
    fileStateCache[fileStates[i].inode] = fileStates[i];
  }

  return fileStateCache;
}

bool FileStateDatabase::insertFileState(FileState fileState){
  std::stringstream query;
  query << "INSERT INTO statedb (path,modtime,inode) VALUES (" 
	<< "'" << fileState.path << "'," 
	<< fileState.modtime << "," 
	<< fileState.inode << ")";
  dbg_printc(LOG_DBG, "FileStateDatabase", "updateState", "Insert fileState: \"%s\" ", query.str().c_str());
  return executeQuery(query.str(), FileStateDatabase::printQueryResult, 0);

}

bool FileStateDatabase::updateFileState(FileState fileState){
  std::stringstream query;
  query << "UPDATE statedb SET modtime=" << (int) fileState.modtime << " WHERE inode=" << (int) fileState.inode; 
  dbg_printc(LOG_DBG, "FileStateDatabase", "updateState", "Update fileState: \"%s\" ", query.str().c_str());
  return executeQuery(query.str(), FileStateDatabase::printQueryResult, 0);

}

bool FileStateDatabase::resetFileState(){
  return executeQuery("DELETE FROM statedb", FileStateDatabase::printQueryResult, 0);
}

bool FileStateDatabase::updateState(std::string rootDirectory){
  boost::filesystem::recursive_directory_iterator it(rootDirectory, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  // Create fileStateCache
  std::map<int, FileState> fileStateCache = createFileStateCache();

  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();
    if(boost::filesystem::is_regular_file(*it)){
      FileState fileState = createFileState(*it);
	
      auto it = fileStateCache.find(fileState.inode);
      if(it == fileStateCache.end()){
	insertFileState(fileState);
      }
      else if(it->second.modtime < fileState.modtime){
	fileState.modtime = fileState.modtime;
	updateFileState(fileState);
      }

    }
    it++;
  }
  return true;

}

