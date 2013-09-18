#include <WebdavSyncManager.h>
#include <WebdavClient.h>
#include <SyncType.h>
#include <boost/filesystem.hpp> /* filesystem, is_directory, file_size, last_write_time */

WebdavSyncManager::WebdavSyncManager( std::string destFolder, 
				      SyncType syncType, 
				      std::string destUser, 
				      std::string destHost,
				      std::string destPort,
				      std::string destPass) :
  SyncManager(destFolder, syncType),
  mDestFolder(destFolder),
  mWebdavClient(destHost, destPort, destUser, destPass),
  mDb("test")
  {

}

bool WebdavSyncManager::syncSourceFolder(std::string rootPath){
  dbg_printc(LOG_DBG, "WebdavSyncManager","syncSourceFolder", rootPath.c_str());

  if(mSyncType == DOB_BACKUP){
    std::vector<std::pair<FileState, ModState> > modStates = mDb.updatedb(rootPath);

    for(auto it = modStates.begin(); it != modStates.end(); ++it){
      FileState fs     = it->first;
      ModState ms      = it->second;
      bool is_dir      = fs.is_dir;
      std::string path = fs.path;

      bool result = false;
      switch(ms){

      case FS_MODIFY:
      case FS_CREATE:
	if(is_dir)
	  result = pushFolder(rootPath, boost::filesystem::path(path));
	else 
	  result = pushFile(rootPath, boost::filesystem::path(path));
	break;

      case FS_CREATE:
	if(is_dir)
	  result = pushFolder(rootPath, boost::filesystem::path(path));
	else 
	  result = pushFile(rootPath, boost::filesystem::path(path));
	break;

      case FS_DELETE:
	if(is_dir)
	  result = removeFolder(rootPath, boost::filesystem::path(path + "/"));
	else 
	  result = removeFolder(rootPath, boost::filesystem::path(path));
	break;
      default:
	break;
      };
      if(result)
	mDb.executeQuery(ms, fs);
      

      // DEBUG
      //dbg_printc(LOG_DBG, "WebdavSyncManager","syncSourceFolder","Modstate %s %d %d %d %d", it->first.path.c_str(), it->first.modtime, it->first.inode , it->first.is_dir, it->second);
    }
    return true;
  }
  else {
    dbg_printc(LOG_ERR, "WebdavSyncManager", "syncSourceFolder", "Choosen SyncType is not implemented for WebdavSyncManager. Please change in configfile.");
  }

  return false;
  
}


bool WebdavSyncManager::syncFolder(std::string rootPath, std::string relativePath, std::string folder){
  return pushFolderRecursively(rootPath, relativePath + folder, false);
  
}

bool WebdavSyncManager::syncFile(std::string rootPath, std::string fullPath){
  boost::filesystem::path fullPathTmp(fullPath);
  if(boost::filesystem::is_directory(fullPathTmp))
    return pushFolderRecursively(rootPath, fullPath, false);
  if(boost::filesystem::is_regular_file(fullPathTmp))
    return pushFile(rootPath, boost::filesystem::path(fullPath));

  return false;
}

bool WebdavSyncManager::removeFolder(std::string rootPath, std::string relativePath, std::string folder){
  return removeFolder(rootPath, boost::filesystem::path(relativePath + folder));
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

// @todo Check for modtime, can reduce time on big amount of data
bool WebdavSyncManager::pushFolderRecursively(std::string rootPath, std::string fullPath, bool checkExistance){
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolderRecursively", fullPath.c_str());
  // symlink_option::recurse tells the directory_iterator to follow symlinks
  boost::filesystem::recursive_directory_iterator it(fullPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  pushFolder(rootPath, boost::filesystem::path(fullPath));

  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();
    std::string uri = mDestFolder + currentPath.substr(rootPath.length(), currentPath.length());
    
    bool uriExists = false;
    if(checkExistance)
      uriExists = mWebdavClient.exists(uri);

    if(!uriExists){
      if(boost::filesystem::is_directory(*it) && !hasSymlinkLoop(*it)){
	pushFolder(rootPath, *it);
     
      }
      else if(boost::filesystem::is_symlink(*it)){
	pushFolder(rootPath, *it);

      }
      else if(boost::filesystem::is_regular_file(*it)){
	pushFile(rootPath, *it);

      }

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

/**
 * @brief Removes folder or file on remote webdavserver. Folder need to end with '/'
 *
 **/
bool WebdavSyncManager::removeFolder(std::string rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestFolder + fullPath.string().substr(rootPath.length(), fullPath.string().length());
  dbg_printc(LOG_DBG, "WebdavSyncManager","removeFolder", "%s", uri.c_str());   
  return mWebdavClient.rm(uri);
}

bool WebdavSyncManager::pullFolderRecursively(std::string rootPath, std::string uri){
  dbg_printc(LOG_DBG, "WebdavSyncManager", "pullFolderRecursively", "%s", uri.c_str());
  std::vector<WebdavPath> paths = mWebdavClient.ls(uri);
  for(unsigned i = 0; i < paths.size(); ++i){
    std::string localDestination = rootPath + paths[i].path.substr(mDestFolder.length(), paths[i].path.length());
    std::string mkdirQuery = "mkdir -p " + localDestination;
    if(!boost::filesystem::exists(localDestination)){
      if(paths[i].isDirectory()){
	system(mkdirQuery.c_str());
	pullFolderRecursively(rootPath, paths[i].path);
      }
      else if(paths[i].isFile()){
	mWebdavClient.get(paths[i].path, localDestination);

      }

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
  return false;

}
