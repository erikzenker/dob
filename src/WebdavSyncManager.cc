#include <WebdavSyncManager.h>
#include <WebdavClient.h>
#include <WebdavPath.h>
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
  mDestFolder(destFolder),
  mWebdavClient(destHost, destUser, "")
  {

}

bool WebdavSyncManager::syncSourceFolder(std::string rootPath){
  //return pullFolderRecursively(mDestFolder, mDestFolder);
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

  pushFolder(rootPath, fullPath);

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

bool WebdavSyncManager::pushFile(std::string rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestFolder + fullPath.string().substr(rootPath.length(), fullPath.string().length());
  std::string localSource = fullPath.string();
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "Push file %s to %s", localSource.c_str(), uri.c_str());   
  return mWebdavClient.put(uri, localSource);
}

bool WebdavSyncManager::pushFolder(std::string rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestFolder + fullPath.string().substr(rootPath.length(), fullPath.string().length());
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolder", "%s", uri.c_str());   
  return mWebdavClient.mkdir(uri);
}

bool WebdavSyncManager::removeFolder(std::string rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestFolder + fullPath.string().substr(rootPath.length(), fullPath.string().length());
  dbg_printc(LOG_DBG, "WebdavSyncManager","removeFolder", "%s", uri.c_str());   
  return mWebdavClient.del(uri);
}

 bool WebdavSyncManager::pullFolderRecursively(std::string rootPath, std::string fullPath){
   //WebdavClient webdavClient(mDestHost, mDestUser, ""); // "http://" must be replaced or tolerated
   dbg_printc(LOG_DBG, "WebdavSyncManager", "pullFolderRecursively", "%s", fullPath.c_str());
   WebdavClient webdavClient("erikspi.mooo.com", mDestUser, "");
   std::vector<WebdavPath> paths = webdavClient.ls(fullPath);
   for(unsigned i = 0; i < paths.size(); ++i){
     if(paths[i].isDirectory()){
       // Create local Path
       pullFolderRecursively(rootPath, paths[i].getPath());
     }
   }

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
