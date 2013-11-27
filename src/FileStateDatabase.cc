#include <FileStateDatabase.h>
#include <utility> /* std::pair */
#include <sstream> /* std::stringstream */
#include <sys/stat.h> /* stat, lstat */
#include <vector>
#include <boost/filesystem.hpp>
#include <iostream>

FileStateDatabase::FileStateDatabase(std::string dataBaseLocation, boost::filesystem::path rootPath) 
  : mRootPath(rootPath){
  
  int error = sqlite3_open(dataBaseLocation.c_str(), &mDataBase);
  executeQuery("CREATE TABLE statedb (path varchar(512), modtime int, inode int, is_dir boolean);", FileStateDatabase::noAction, 0);
  if(error){
    sqlite3_close(mDataBase);

  }

}

FileStateDatabase::~FileStateDatabase(){
  sqlite3_close(mDataBase);

}

int FileStateDatabase::noAction(void *NotUsed, int argc, char **argv, char **azColName){
  for(int i=0; i < argc; ++i){
    //do nothing
  }
  return 0;
}


int FileStateDatabase::getFileStates(void *fileStates, int argc, char **argv, char **azColName){
  FileState fileState = {"", 0,0,0};

  std::string path = argv[0];
  fileState.path = path;
  fileState.modtime = atoi(argv[1]);
  fileState.inode = atoi(argv[2]);
  fileState.is_dir = atoi(argv[3]);

  ((std::vector<FileState>*)fileStates)->push_back(fileState);
  return 0;
}
  
bool FileStateDatabase::executeQuery(std::string query, int (*callback)(void*,int,char**,char**) ,void* userData){
  char *errorMsg = 0;
  //std::cout << "Execute:" << query << std::endl;
  int error = sqlite3_exec(mDataBase, query.c_str(), callback, userData, &errorMsg);
  if(error){
    //std::cout << "SQL error: "<< errorMsg << std::endl;
    sqlite3_free(errorMsg);
    return false;
  }
  return true;

}

FileState FileStateDatabase::createFileState(boost::filesystem::path path){
  FileState fileState = {"",0,0,0};

  struct stat buffer;
  fileState.path    = path.string();
  if(boost::filesystem::exists(fileState.path)){
    fileState.modtime = boost::filesystem::last_write_time(path);
    fileState.inode   = lstat(path.c_str(), &buffer)? 0 : buffer.st_ino;
    fileState.is_dir  = boost::filesystem::is_directory(path);
  }
  return fileState;
}

std::map<int, FileState> FileStateDatabase::createFileStateCache(){
  std::vector<FileState> fileStates;
  std::map<int, FileState> fileStateCache;
  executeQuery("SELECT path, modtime, inode, is_dir FROM statedb", FileStateDatabase::getFileStates, &fileStates);
  for(unsigned i = 0; i < fileStates.size(); ++i){
    fileStateCache[fileStates[i].inode] = fileStates[i];
  }

  return fileStateCache;
}
bool FileStateDatabase::propagateUpdate(boost::filesystem::path path, ModState ms){
  std::pair<FileState, ModState> update(createFileState(path), ms);
  return propagateUpdate(update);

}


bool FileStateDatabase::propagateUpdate(std::pair<FileState, ModState> update){
  FileState fileState = update.first;
  ModState modState   = update.second;

 switch(modState){
  case FS_CREATE:
    return insertFileState(fileState);
  case FS_MODIFY:
    return  updateFileState(fileState);
  case FS_DELETE:
    return deleteFileState(fileState);
  default:
    break;

  };
  return false;

}

/**
 * @brief inserts a file in database
 * @return true on succes
 *
 **/
bool FileStateDatabase::insertFileState(FileState fileState){
  std::stringstream query;
  query << "INSERT INTO statedb (path,modtime,inode,is_dir) VALUES (" 
	<< "'" << fileState.path.string().c_str() << "'," 
	<< fileState.modtime << "," 
	<< fileState.inode << ","
	<< fileState.is_dir << ")";

  return executeQuery(query.str(), FileStateDatabase::noAction, 0);

}

/**
 * @brief Updates a file in database
 * @return true on succes
 *
 **/
bool FileStateDatabase::updateFileState(FileState fileState){
  std::stringstream query;
  query << "UPDATE statedb SET modtime=" << (int) fileState.modtime << " WHERE inode=" << (int) fileState.inode; 
  return executeQuery(query.str(), FileStateDatabase::noAction, 0);

}

/**
 * @brief Removes a file from database
 * @return true on succes
 *
 **/
bool FileStateDatabase::deleteFileState(FileState fileState){
  std::stringstream query;
  query << "DELETE FROM statedb WHERE inode=" << (int) fileState.inode; 
  return executeQuery(query.str(), FileStateDatabase::noAction, 0);
}

/**
 * @brief Removes all entries from filestate database
 *
 **/
bool FileStateDatabase::resetdb(){
  return executeQuery("DELETE FROM statedb", FileStateDatabase::noAction, 0);
}

/**
 * @brief Generates a list of new, updated or deleted files since last call of getUpdates.
 *        The changes have to be iterated and executed through insertFileState, 
 *        updateFileState and deleteFileState.
 *
 * @return a pair of FileState and ModState(new, update, delete)
 *
 **/
std::vector<std::pair<FileState, ModState> > FileStateDatabase::getUpdates(){
  boost::filesystem::recursive_directory_iterator it(mRootPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  // Modification state of changed files since last sync
  std::vector<std::pair<FileState, ModState> > modStates;

  // Create fileStateCache
  std::map<int, FileState> fileStateCache = createFileStateCache();

  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();
    FileState fileState = createFileState(*it);

    if(boost::filesystem::is_regular_file(*it)){
	
      auto cacheIt = fileStateCache.find(fileState.inode);
      // New file since last sync
      if(cacheIt == fileStateCache.end()){
	modStates.push_back(std::pair<FileState, ModState>(fileState, FS_CREATE));
	
      }
      // Changed file since last sync
      else if(cacheIt->second.modtime < fileState.modtime){
	fileState.modtime = fileState.modtime;
	fileStateCache.erase(cacheIt);
	modStates.push_back(std::pair<FileState, ModState>(fileState, FS_MODIFY));
      }
      // Unchanged file
      else {
	fileStateCache.erase(cacheIt);

      }

    }
    else if(boost::filesystem::is_directory(*it)){
      auto cacheIt = fileStateCache.find(fileState.inode);
      if(cacheIt == fileStateCache.end()){
    	modStates.push_back(std::pair<FileState, ModState>(fileState, FS_CREATE));

      }
      else {
	fileStateCache.erase(cacheIt);

      }
    }
    it++;

  }

  // Remaining files/directories in FileCache were deleted since last sync
  std::vector<std::pair<FileState, ModState> > deletedFiles;
  for(auto cacheIt = fileStateCache.begin(); cacheIt != fileStateCache.end(); ++cacheIt){
    FileState fs = cacheIt->second;
    modStates.push_back(std::pair<FileState, ModState>(fs, FS_DELETE));
  }
  
  return modStates;
}

std::string FileStateDatabase::modStateToString(const ModState modState){
  switch(modState){
  case FS_CREATE:
    return std::string("FS_CREATE");
  case FS_MODIFY:
    return std::string("FS_MODIFY");
  case FS_DELETE:
    return std::string("FS_DELETE");
  default:
    break;

  };
  return std::string("");

}

FileState FileStateDatabase::getFileState(boost::filesystem::path path){

};

