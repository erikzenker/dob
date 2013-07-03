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

bool FileIndex::indexRecursively(std::string indexPath, std::vector<std::string>* filesToUpdate){
  boost::filesystem::path path(indexPath);
  boost::filesystem::recursive_directory_iterator it(path);
  boost::filesystem::recursive_directory_iterator end;
  
  while(it != end){

    if(boost::filesystem::is_regular_file(*it)){
      indexFile(*it, filesToUpdate);
    }
    ++it;

  }
  return indexFile(path, filesToUpdate);

}

// TODO Watch permissions for size, lastWriteTime
bool FileIndex::indexFile(boost::filesystem::path path, std::vector<std::string>* filesToUpdate){
  dbg_printc(LOG_DBG, "IndexFile", "indexFile", "%s", path.string().c_str());
  std::string collectionName = "dob.testing";
  std::string relativePath = path.string();
  relativePath.erase(0, mRootPath.size());
  bool isDir = boost::filesystem::is_directory(path);
  uint size = (isDir) ? 0 : boost::filesystem::file_size(path);
  std::time_t lastWriteTime = boost::filesystem::last_write_time(path);
  uint revision = 0;
  std::string root = mRootPath;
  std::string modified = "";

  // Build database entry
  mongo::BSONObjBuilder builder;
  builder.append("path", relativePath);
  builder.append("is_dir", isDir);
  builder.append("size", size);
  builder.append("revision", revision);
  builder.append("root", root);
  builder.append("modified", modified);
  builder.append("last_write_time", std::asctime(std::localtime(&lastWriteTime)));
  mongo::BSONObj dataToWrite = builder.obj();

  // Test for existing entry
  std::auto_ptr<mongo::DBClientCursor> cursor = dbClientConnection->query(collectionName, BSON("path" << relativePath));
  if(!cursor->more()){
    // Entry does not exist --> creat new one
    dbClientConnection->insert(collectionName, dataToWrite);
  }
  else{
    // Entry exist --> possible update
    mongo::BSONObj dataBaseFile = cursor->next();
    std::string lastWriteTimeFromDatabase = dataBaseFile.getStringField("last_write_time");
    if(lastWriteTimeFromDatabase.compare(std::asctime(std::localtime(&lastWriteTime))) != 0){
      dbClientConnection->update(collectionName, BSON("path" << relativePath), dataToWrite, true, false);
      filesToUpdate->push_back(mRootPath + relativePath);
    }
  }
  return true;

}

void FileIndex::setRootPath(std::string rootPath){
  mRootPath = rootPath;

}
