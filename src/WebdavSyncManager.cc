#include <WebdavSyncManager.h>
#include <WebdavClient.h>
#include <SyncType.h>
#include <stdlib.h>
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
  mWebdavClient(destHost, atoi(destPort.c_str()), destUser, destPass)
  {

}

bool WebdavSyncManager::pushFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  uri = replaceSubstring(uri, " ", "%20");
  std::string localSource = fullPath.string();

  if(mWebdavClient.put(uri, localSource)){
    dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "Push file %s to %s", localSource.c_str(), uri.c_str());   
    return true;
  }
  else {
    dbg_printc(LOG_DBG, "WebdavSyncManager","pushFile", "Push file %s to %s error ! %s!", localSource.c_str(), uri.c_str(), mWebdavClient.getLastError().c_str()); 
  }
  return false;
}

bool WebdavSyncManager::pushDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath, const bool recursive){
  dbg_printc(LOG_DBG, "WebdavSyncManager","pushDir", fullPath.c_str());
  // symlink_option::recurse tells the directory_iterator to follow symlinks
  boost::filesystem::recursive_directory_iterator it(fullPath, boost::filesystem::symlink_option::recurse);
  boost::filesystem::recursive_directory_iterator end;


  mkdir(rootPath, fullPath);
  
  if(!recursive)
    return true;

  while(it != end){
    std::string currentPath = ((boost::filesystem::path)*it).string();
    std::string uri = mDestPath.string() + currentPath.substr(rootPath.string().length(), currentPath.length());
    
    if(boost::filesystem::is_directory(*it)){
      mkdir(rootPath, *it);
     
    }
    else if(boost::filesystem::is_symlink(*it)){
      mkdir(rootPath, *it);

    }
    else if(boost::filesystem::is_regular_file(*it)){
      pushFile(rootPath, *it);

    }
    ++it;

  }

  return true;
}

bool WebdavSyncManager::removeFile(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  return removeDir(rootPath, fullPath);
}

bool WebdavSyncManager::removeDir(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
  dbg_printc(LOG_DBG, "WebdavSyncManager","removeDir", "%s", uri.c_str());   
  if(mWebdavClient.rm(uri)){
    return true;
  }
  return false;

}

bool WebdavSyncManager::mkdir(boost::filesystem::path rootPath, boost::filesystem::path fullPath){
  std::string uri = mDestPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());

  if(mWebdavClient.mkdir(uri)){
    dbg_printc(LOG_DBG, "WebdavSyncManager","mkdir", "%s", uri.c_str());   
    return true;
  }
  return false;
}

// bool WebdavSyncManager::pullFolderRecursively(boost::filesystem::path rootPath, std::string uri){
//   // dbg_printc(LOG_DBG, "WebdavSyncManager", "pullFolderRecursively", "%s", uri.c_str());
//   // std::vector<WebdavPath> paths = mWebdavClient.ls(uri);
//   // for(unsigned i = 0; i < paths.size(); ++i){
//   //   std::string localDestination = rootPath + paths[i].path.substr(mDestFolder.length(), paths[i].path.length());
//   //   std::string mkdirQuery = "mkdir -p " + localDestination;
//   //   if(!boost::filesystem::exists(localDestination)){
//   //     if(paths[i].isDirectory()){
//   // 	system(mkdirQuery.c_str());
//   // 	pullFolderRecursively(rootPath, paths[i].path);
//   //     }
//   //     else if(paths[i].isFile()){
//   // 	mWebdavClient.get(paths[i].path, localDestination);

//   //     }

//   //   }

//   // }
//   return true;

// }

std::string WebdavSyncManager::replaceSubstring(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

// bool WebdavSyncManager::hasSymlinkLoop(boost::filesystem::path path){
//   // struct stat buffer;
//   // int status;
//   // int inode;
//   // errno = 0;

//   // if(boost::filesystem::is_symlink(path)){
//   //   status = lstat(path.string().c_str(), &buffer);
//   //   if(!status){
//   //     inode = buffer.st_ino;
//   //     auto it = mSymlinks.find(inode);
//   //     if(it == mSymlinks.end()){
//   // 	mSymlinks[inode] = path.string();
//   //     }
//   //     else{
//   // 	dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Found loop in filesystem Inode %d %s folder wont be followed further", buffer.st_ino, path.string().c_str());   
//   // 	return true;

//   //     }
//   //   }
//   //   else{
//   //     dbg_printc(LOG_WARN, "WebdavSyncManager","hasSymlinkLoop", "Error on lstat %s ernno %d", path.string().c_str(), errno);   
//   //   }
//   // }
//   return false;

// }
