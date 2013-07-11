#include "WebdavSyncManager.h"
#include <algorithm>


WebdavSyncManager::WebdavSyncManager( std::string destFolder, 
				    std::string syncType, 
				    std::string destUser, 
				    std::string destHost,
				    std::string destPort) :
  SyncManager(destFolder, syncType),
  mDestPort(destPort),
  mDestUser(destUser),
  mDestHost(destHost),
  mDestFolder(destFolder)
  {
    mFileIndex = new FileIndex("");

}

bool WebdavSyncManager::syncSourceFolder(std::string rootPath){
  return pushFolderRecursively(rootPath, boost::filesystem::path(rootPath));
}


bool WebdavSyncManager::syncFolder(std::string rootPath, std::string syncFolder, std::string folder){
  return pushFolderRecursively(rootPath, boost::filesystem::path(syncFolder + folder));
  
}

bool WebdavSyncManager::syncFile(std::string rootPath, std::string fullPath){
  boost::filesystem::path fullPathTmp(fullPath);
  if(boost::filesystem::is_directory(fullPathTmp))
    return pushFolderRecursively(rootPath, boost::filesystem::path(fullPath));
  if(boost::filesystem::is_regular_file(fullPathTmp))
    return pushFile(rootPath, boost::filesystem::path(fullPath));

  return false;
}

/**
 * 
 *
 **/
bool WebdavSyncManager::removeFolder(std::string rootPath, std::string syncFolder, std::string folder){
  return removeFolder(rootPath, boost::filesystem::path(syncFolder + folder));
}

/**
 * @todo fill with content
 *
 **/
bool WebdavSyncManager::checkDestination(){
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool WebdavSyncManager::setupDestination(){
  return true;
}

bool WebdavSyncManager::pushFolderRecursively(std::string rootPath, boost::filesystem::path fullPath){
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolderRecursively", fullPath.c_str());
  boost::filesystem::recursive_directory_iterator it(fullPath);
  boost::filesystem::recursive_directory_iterator end;

  createFolder(rootPath, fullPath);

  while(it != end){
    //dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolderRecursively", "Check");
    if(boost::filesystem::is_directory(*it) && !hasSymlinkLoop(*it)){
      pushFolderRecursively(rootPath, *it);
    }
    else if(boost::filesystem::is_regular_file(*it)){
      pushFile(rootPath, *it);
    }
    ++it;

  }

  return true;
}

// @todo errorhandling
bool WebdavSyncManager::pushFile(std::string rootPath, boost::filesystem::path fullPath){
  std::string fullPathUrl = replaceSubstring(fullPath.string(), " ", "%20");
  std::string curlQuery = "curl -T \'" + fullPath.string() + "\' " + mDestHost + mDestFolder + fullPathUrl.substr(rootPath.length(), fullPathUrl.length());

  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "%s", curlQuery.c_str());   
  system(curlQuery.c_str());

  return true;
}

// @todo Error handling
bool WebdavSyncManager::createFolder(std::string rootPath, boost::filesystem::path fullPath){
  std::string fullPathUrl = replaceSubstring(fullPath.string(), " ", "%20");
  std::string curlQuery;
  curlQuery
    .append("curl -X MKCOL ")
    .append(mDestHost)
    .append(mDestFolder)
    .append(fullPathUrl.substr(rootPath.length(), fullPathUrl.length()));

  dbg_printc(LOG_DBG, "WebdavSyncManager","createFolder", "%s", curlQuery.c_str());   
  system(curlQuery.c_str());

  return true;
}

bool WebdavSyncManager::removeFolder(std::string rootPath, boost::filesystem::path fullPath){
  std::string fullPathUrl = replaceSubstring(fullPath.string(), " ", "%20");
  if(fullPathUrl.at(fullPathUrl.size() - 1) != '/'){
    fullPathUrl.append("/");
  }
  std::string curlQuery;
  try{
  curlQuery
    .append("curl -X DELETE ")
    .append(mDestHost)
    .append(mDestFolder)
    .append(fullPathUrl.substr(rootPath.length(), fullPathUrl.length()));
  }
  catch(int e){
    dbg_printc(LOG_DBG, "WebdavSyncManager","removeFolder", "Exception %d", e);   
  }
  dbg_printc(LOG_DBG, "WebdavSyncManager","removeFolder", "%s", curlQuery.c_str());   
  system(curlQuery.c_str());
  return true;

}

std::string WebdavSyncManager::replaceSubstring(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

bool WebdavSyncManager::hasSymlinkLoop(boost::filesystem::path path){
  struct stat buffer;
  int status;
  int inode;
  errno = 0;

  if(boost::filesystem::is_symlink(path)){
    status = lstat(path.string().c_str(), &buffer);
    if(!status){
      inode = buffer.st_ino;
      auto it = mSymlinks.find(inode);
      if(it == mSymlinks.end()){
	mSymlinks[inode] = path.string();
      }
      else{
	dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Found loop in filesystem Inode %d %s folder wont be followed further", buffer.st_ino, path.string().c_str());   
	return true;

      }
    }
    else{
      dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Error on lstat %s ernno %d", path.string().c_str(), errno);   
    }
  }
  return false;;

}
