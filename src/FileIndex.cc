#include <FileIndex.h>

FileIndex::FileIndex(std::string rootPath): 
  mRootPath(rootPath){
  try {
    dbClientConnection = new mongo::DBClientConnection;
    dbClientConnection->connect("localhost");

  } catch( const mongo::DBException &e ) {
    //dbg_printc(LOG_DBG, "FileIndex", "FileIndex", "%s", e.what.c_str());

  }

}

FileIndex::~FileIndex(){


}

bool FileIndex::indexRecursively(std::string path){
  DIR * directory;
  int error = 0;
  std::string nextFile = "";
  struct dirent * ent;
  struct stat64 my_stat;

  directory = opendir(path.c_str());
  if(!isDir(path)){
    closedir(directory);
    return indexFile(path);
  }

  if(path[path.size()-1] != '/'){
    path.append("/");
  }
  ent = readdir(directory);

  // Index each directory within this directory
  while(ent){
    if((0 != strcmp(ent->d_name, ".")) && (0 != strcmp(ent->d_name, ".."))){
      nextFile = path;
      nextFile.append(ent->d_name);
	
      // Check the File/Folder for acces
      if(lstat64(nextFile.c_str(), &my_stat) == -1){
	error = errno;
	dbg_printc(LOG_ERR, "FileIndex", "indexRecursively", "C Error on fstat %s, %d", nextFile.c_str(), error);
	if (error != EACCES){
	  error = errno;
	  closedir(directory);
	  return false;

	}

      }
      // Watch a folder recursively
      else if(S_ISDIR(my_stat.st_mode ) || S_ISLNK( my_stat.st_mode )) {
	nextFile.append("/");
	bool status = indexRecursively(nextFile);
	if (!status){
	  closedir(directory);
	  return false;
	}
	
      }
      else{
	// index all files in this folder
	indexFile(nextFile);
      }

    }
    ent = readdir(directory);
    error = 0;
  }
  closedir(directory);
  return indexFile(path);
}

bool FileIndex::indexFile(std::string path){
  dbg_printc(LOG_DBG, "IndexFile", "indexFile", "%s", path.c_str());
  std::string relativePath = path;
  relativePath.erase(0, mRootPath.size());
  bool is_dir = isDir(path);
  uint size  = getFileSize(path);
  uint revision = 0;
  std::string root = mRootPath;
  std::string modified = "";


  // Build database entry
  mongo::BSONObjBuilder b;
  b.append("path", relativePath);
  b.append("is_dir", is_dir);
  b.append("size", size);
  b.append("revision", revision);
  b.append("root", root);
  b.append("modified", modified);
  mongo::BSONObj p = b.obj();

  // Update database
  dbClientConnection->update("dob.testing", BSON("path" << relativePath), p, true, false);
  return true;
}

bool FileIndex::isDir(std::string path){
  DIR* directory;
  directory = opendir(path.c_str());
  if(!directory) {
    int error = errno;
    if(error == ENOTDIR){

    }
    else {
      dbg_printc(LOG_WARN, "FileIndex","isDir", "Couldn´t not opendir %s, Errno: %d", path.c_str(), error);


    }
    closedir(directory);
    return false;

  }
  closedir(directory);
  return true;

}

uint FileIndex::getFileSize(std::string path){
  struct stat64 my_stat;
  if(lstat64(path.c_str(), &my_stat) < 0){
    int error = errno;
    dbg_printc(LOG_WARN, "FileIndex","getFileSize", "Can't get FileSize %s, Errno: %d", path.c_str(), error);
    return 0;
  }
  return my_stat.st_size;

}
