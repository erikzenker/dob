#include <WebdavSyncManager.h>
#include <WebdavClient.h>
#include <SyncType.h>
#include <stdlib.h>
#include <FileStateDatabase.h>
#include <boost/filesystem.hpp> /* filesystem, is_directory, file_size, last_write_time */
#include <dbg_print.h>

WebdavSyncManager::WebdavSyncManager(boost::filesystem::path scanPath, 
				     std::string destFolder, 
				     SyncType syncType, 
				     std::string destUser, 
				     std::string destHost,
				     std::string destPort,
				     std::string destPass) :
  SyncManager(destFolder, syncType),
  mWebdavClient(destHost, atoi(destPort.c_str()), destUser, destPass),
  mFileStateDatabase(".dob_fileState", scanPath)
  {

}

bool WebdavSyncManager::syncInitial(boost::filesystem::path rootPath){
  dbg_printc(LOG_DBG, "WebdavSyncManager","syncSourceFolder", rootPath.c_str());

  if(mSyncType == DOB_BACKUP){
    std::vector<std::pair<FileState, ModState> > updates = mFileStateDatabase.getUpdates();

    for(auto update = updates.begin(); update != updates.end(); ++update){
      FileState fs     = update->first;
      ModState ms      = update->second;
      bool is_dir      = fs.is_dir;
      boost::filesystem::path path = fs.path;
      bool result = false;
      switch(ms){

      case FS_MODIFY:
	if(is_dir)
	  result = pushFolder(rootPath, path, ms);
	else 
	  result = pushFile(rootPath, path, ms);
	break;

      case FS_CREATE:
	if(is_dir)
	  result = pushFolder(rootPath, path, ms);
	else 
	  result = pushFile(rootPath, path, ms);
	break;

      case FS_DELETE:
	if(is_dir)
	  result = removeFolder(rootPath, path/"/");
	else 
	  result = removeFolder(rootPath, path);
	break;
      default:
	break;
      };
      
      if(!result)
	dbg_printc(LOG_WARN, "WebdavSyncManager","syncSourceFolder","Not able to execute ModState for %s! %s", path.c_str(), mWebdavClient.getLastError().c_str());
    }
    return true;
  }
  dbg_printc(LOG_ERR, "WebdavSyncManager", "syncSourceFolder", "Choosen SyncType is not implemented for WebdavSyncManager. Please change in configfile.");
  return false;
}


bool WebdavSyncManager::syncFolder(boost::filesystem::path rootPath, boost::filesystem::path relativePath){
  return pushFolderRecursively(rootPath, relativePath, false);
  
}

bool WebdavSyncManager::syncFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms){
  if(boost::filesystem::is_symlink(fullPath) && boost::filesystem::is_directory(fullPath))
    return pushFolderRecursively(rootPath, fullPath, false);
  if(boost::filesystem::is_regular_file(fullPath))
    return pushFile(rootPath, fullPath, ms);
  return false;
}

// @todo Check for modtime, can reduce time on big amount of data
bool WebdavSyncManager::pushFolderRecursively(boost::filesystem::path rootPath, boost::filesystem::path fullPath, bool checkExistance){
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolderRecursively", fullPath.c_str());
  // symlink_option::recurse tells the directory_iterator to follow symlinks
  boost::filesystem::recursive_directory_iterator it(fullPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;

  pushFolder(rootPath, fullPath, FS_CREATE);

  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();
    std::string uri = mDestPath.string() + currentPath.substr(rootPath.string().length(), currentPath.length());
    
    bool uriExists = false;
    if(checkExistance)
      uriExists = mWebdavClient.exist(uri);

    if(!uriExists){
      if(boost::filesystem::is_directory(*it) && !hasSymlinkLoop(*it)){
  	pushFolder(rootPath, *it, FS_CREATE);
     
      }
      else if(boost::filesystem::is_symlink(*it)){
  	pushFolder(rootPath, *it, FS_CREATE);

      }
      else if(boost::filesystem::is_regular_file(*it)){
  	pushFile(rootPath, *it, FS_CREATE);

      }

    }
    ++it;

  }

  return true;
}

bool WebdavSyncManager::pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  uri = replaceSubstring(uri, " ", "%20");
  std::string localSource = fullPath.string();


  if(mWebdavClient.put(uri, localSource)){
    dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "Push file %s to %s", localSource.c_str(), uri.c_str());   
    return mFileStateDatabase.propagateUpdate(fullPath, ms);
  }
  else {
    dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "Push file %s to %s error ! %s!", localSource.c_str(), uri.c_str(), mWebdavClient.getLastError().c_str()); 
    return false;
  }
}

bool WebdavSyncManager::pushFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath, ModState ms){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());

  if(mWebdavClient.mkdir(uri)){
    dbg_printc(LOG_DBG, "WebdavSyncManager","pushFolder", "%s", uri.c_str());   
    mFileStateDatabase.propagateUpdate(fullPath, ms);
    return true;
  }
  return false;
}

/**
 * @brief Removes folder or file on remote webdavserver. Folder need to end with '/'
 *
 **/
bool WebdavSyncManager::removeFolder(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  dbg_printc(LOG_DBG, "WebdavSyncManager","removeFolder", "%s", uri.c_str());   
  if(mWebdavClient.rm(uri)){
    mFileStateDatabase.propagateUpdate(fullPath, FS_DELETE);
    return true;
  }
  return false;
}

bool WebdavSyncManager::pullFolderRecursively(boost::filesystem::path rootPath, std::string uri){
  // dbg_printc(LOG_DBG, "WebdavSyncManager", "pullFolderRecursively", "%s", uri.c_str());
  // std::vector<WebdavPath> paths = mWebdavClient.ls(uri);
  // for(unsigned i = 0; i < paths.size(); ++i){
  //   std::string localDestination = rootPath + paths[i].path.substr(mDestFolder.length(), paths[i].path.length());
  //   std::string mkdirQuery = "mkdir -p " + localDestination;
  //   if(!boost::filesystem::exists(localDestination)){
  //     if(paths[i].isDirectory()){
  // 	system(mkdirQuery.c_str());
  // 	pullFolderRecursively(rootPath, paths[i].path);
  //     }
  //     else if(paths[i].isFile()){
  // 	mWebdavClient.get(paths[i].path, localDestination);

  //     }

  //   }

  // }
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
  // struct stat buffer;
  // int status;
  // int inode;
  // errno = 0;

  // if(boost::filesystem::is_symlink(path)){
  //   status = lstat(path.string().c_str(), &buffer);
  //   if(!status){
  //     inode = buffer.st_ino;
  //     auto it = mSymlinks.find(inode);
  //     if(it == mSymlinks.end()){
  // 	mSymlinks[inode] = path.string();
  //     }
  //     else{
  // 	dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Found loop in filesystem Inode %d %s folder wont be followed further", buffer.st_ino, path.string().c_str());   
  // 	return true;

  //     }
  //   }
  //   else{
  //     dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Error on lstat %s ernno %d", path.string().c_str(), errno);   
  //   }
  // }
  return false;

}
