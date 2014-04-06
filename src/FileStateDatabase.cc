#include <FileStateDatabase.h>
#include <FileState.h>
#include <utility> /* std::pair */
#include <sstream> /* std::stringstream */
#include <sys/stat.h> /* stat, lstat */
#include <vector>
#include <boost/filesystem.hpp>
#include <iostream>
#include <exception>

FileStateDatabase::FileStateDatabase(std::vector<std::string> ignoredDirectories, std::string dataBaseLocation, boost::filesystem::path rootPath) : 
  mRootPath(rootPath),
  mIgnoredDirectories(ignoredDirectories)
{
  
  int error = sqlite3_open(dataBaseLocation.c_str(), &mDataBase);
  executeQuery("CREATE TABLE statedb (path varchar(512), modtime int, inode int, is_dir boolean);", FileStateDatabase::noAction, 0);
  if(error){
    sqlite3_close(mDataBase);

  }
  createFileStateCache();

}

FileStateDatabase::~FileStateDatabase(){
  sqlite3_close(mDataBase);

}

/**
 * Sqlite callback
 */
int FileStateDatabase::noAction(void *NotUsed, int argc, char **argv, char **azColName){
  for(int i=0; i < argc; ++i){
    //do nothing
  }
  return 0;
}

/**
 * Sqlite callback
 */
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
  

/**
 * @brief Executes query in query string and handles query result with 
 *        the callback function. The result of the callback function
 *        will be stored in userData.
 *
 */
bool FileStateDatabase::executeQuery(const std::string query, int (*callback)(void*,int,char**,char**) ,void* userData){
  char *errorMsg = 0;
  int error = sqlite3_exec(mDataBase, query.c_str(), callback, userData, &errorMsg);
  if(error){
    sqlite3_free(errorMsg);
    return false;
  }
  return true;

}

FileState FileStateDatabase::createFileState(boost::filesystem::path path){
  FileState fileState;
  struct stat buffer;
  fileState.path    = path;
  if(boost::filesystem::exists(path)){
    fileState.modtime = boost::filesystem::last_write_time(path);
    fileState.inode   = lstat(path.c_str(), &buffer)? 0 : buffer.st_ino;
    fileState.is_dir  = boost::filesystem::is_directory(path);
  }
  return fileState;
}
void FileStateDatabase::createFileStateCache(){
  std::vector<FileState> fileStates;
  executeQuery("SELECT path, modtime, inode, is_dir FROM statedb", FileStateDatabase::getFileStates, &fileStates);
  for(unsigned i = 0; i < fileStates.size(); ++i){
    mFileStateCache[fileStates[i].path.string()] = fileStates[i];
  }
}

bool FileStateDatabase::propagateUpdate(const boost::filesystem::path path,const ModState ms){
  std::pair<FileState, ModState> update(createFileState(path), ms);
  return propagateUpdate(update);

}


bool FileStateDatabase::propagateUpdate(const std::pair<FileState, ModState> update){
  FileState fileState = update.first;
  ModState modState   = update.second;


  std::cout << "propagateUpdate " << fileState.path << std::endl;
 switch(modState){
  case FS_CREATE:
    return insertFileState(fileState);
  case FS_MODIFY:
    return  updateFileState(fileState);
  case FS_DELETE:
    return deleteFileState(fileState, false);
  default:
    break;

  };
  return false;

}

void FileStateDatabase::propagateUpdateRecursively(const boost::filesystem::path rootPath, const ModState ms){
  try {
    if(ms == FS_DELETE){
      deleteFileState(createFileState(rootPath), true);

    }
    else{
      if(boost::filesystem::is_regular_file(rootPath)){
	propagateUpdate(rootPath, ms);

      }
      else{
	boost::filesystem::recursive_directory_iterator it(rootPath, boost::filesystem::symlink_option::recurse);
	boost::filesystem::recursive_directory_iterator end;

	propagateUpdate(rootPath, ms);

	while(it != end){
	  boost::filesystem::path currentPath = ((boost::filesystem::path)*it);
	  propagateUpdate(currentPath, ms);
	  ++it;

	}

      }

    }

  }
  catch(std::exception e){
    std::cerr << e.what() << std::endl;

  }

}



/**
 * @brief inserts a file in database
 * @return true on succes
 *
 **/
bool FileStateDatabase::insertFileState(const FileState fileState){
  std::stringstream query;
  query << "INSERT INTO statedb (path,modtime,inode,is_dir) VALUES (" 
	<< "'" << fileState.path.string().c_str() << "'," 
	<< fileState.modtime << "," 
	<< fileState.inode << ","
	<< fileState.is_dir << ")";

  bool result = executeQuery(query.str(), FileStateDatabase::noAction, 0);
  if(result)
    setFileState(fileState);

  return result;

}

/**
 * @brief Updates a file in database
 * @return true on succes
 *
 **/
bool FileStateDatabase::updateFileState(const FileState fileState){
  std::stringstream query;
  query << "UPDATE statedb SET modtime=" << (int) fileState.modtime << " WHERE inode=" << (int) fileState.inode; 
  bool result = executeQuery(query.str(), FileStateDatabase::noAction, 0);
  if(result)
    setFileState(fileState);

  return result;

}

/**
 * @brief Removes a file from database
 * @return true on succes
 *
 **/
bool FileStateDatabase::deleteFileState(const FileState fileState, const bool recursive){
  std::stringstream query;
  if(recursive){
    query << "DELETE FROM statedb WHERE path LIKE '" << fileState.path.string() << "%'"; 
  }
  else {
    query << "DELETE FROM statedb WHERE inode=" << (int) fileState.inode; 
  }
  bool result = executeQuery(query.str(), FileStateDatabase::noAction, 0);
  if(result){
    auto cacheIt = mFileStateCache.find(fileState.path.string());
    if(cacheIt != mFileStateCache.end()){
      mFileStateCache.erase(cacheIt);
    }

  }
  return result;
}

/**
 * @brief sets the filestate fs in fileStateCache
 *
 **/
void FileStateDatabase::setFileState(const FileState fs){
  boost::filesystem::path path = fs.path;
  mFileStateCache[path.string()] = fs ;
}

/**
 * @brief gets the filestate for path from fileStatedatabase
 *
 **/
FileState FileStateDatabase::getFileState(const boost::filesystem::path path){
  return mFileStateCache[path.string()];
}

/**
 * @brief Generates a list of new, updated or deleted files since last call of getUpdates.
 *        The changes have to be iterated and executed through insertFileState, 
 *        updateFileState and deleteFileState.
 *
 * @return a pair of FileState and ModState(new, update, delete)
 *
 **/
std::vector<std::pair<FileState, ModState> > FileStateDatabase::updates(){
  boost::filesystem::recursive_directory_iterator it(mRootPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  // Modification state of changed files since last sync
  std::vector<std::pair<FileState, ModState> > modStates;

  // Updates since last call
  std::map<std::string, FileState> updates = mFileStateCache;
  
  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();

    if(!isIgnored(currentPath)){
      FileState fileState = createFileState(currentPath);
      if(boost::filesystem::is_regular_file(*it)){
	
	auto cacheIt = updates.find(fileState.path.string());


	// New file since last sync
	if(cacheIt == updates.end()){
	  modStates.push_back(std::pair<FileState, ModState>(fileState, FS_CREATE));
	
	}
	// Changed file since last sync
	else if(cacheIt->second.modtime < fileState.modtime){
	  fileState.modtime = fileState.modtime;
	  updates.erase(cacheIt);
	  modStates.push_back(std::pair<FileState, ModState>(fileState, FS_MODIFY));
	}
	// Unchanged file
	else {
	  updates.erase(cacheIt);

	}

      }
      else if(boost::filesystem::is_directory(*it)){
	auto cacheIt = updates.find(fileState.path.string());
	if(cacheIt == updates.end()){
	  modStates.push_back(std::pair<FileState, ModState>(fileState, FS_CREATE));

	}
	else {
	  updates.erase(cacheIt);

	}
      }
    }
    it++;

  }

  // Remaining files/directories in FileCache were deleted since last sync
  std::vector<std::pair<FileState, ModState> > deletedFiles;
  for(auto cacheIt = updates.begin(); cacheIt != updates.end(); ++cacheIt){
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

bool FileStateDatabase::isIgnored( std::string file){
  if(mIgnoredDirectories.empty()){
    return false;
  }

  for(unsigned i = 0; i < mIgnoredDirectories.size(); ++i){
    size_t pos = file.find(mIgnoredDirectories[i]);
    if(pos!= std::string::npos){
      return true;
    }
  }
  return false;
}

