#define BOOST_NO_CXX11_SCOPED_ENUMS /* workaround for use of copy_file */
#include <boost/filesystem.hpp>
#include <iostream>

#include <SyncType.h>
#include <LocalSyncManager.h>

namespace fs = boost::filesystem;

fs::path constructDestPath(const fs::path rootPath, fs::path fullPath, fs::path destRootPath);

LocalSyncManager::LocalSyncManager(fs::path destPath, SyncType syncType) :
	SyncManager(destPath, syncType){
	if(!boost::filesystem::exists(destPath)){
		throw std::invalid_argument("Can't initialize SyncManager! destPath does not exist. destPath: " + destPath.string());
	}

}

bool LocalSyncManager::pushFile(fs::path rootPath, fs::path fullPath) const{
	const fs::path fullDestPath = constructDestPath(rootPath, fullPath, destPath);
	std::cout << "pushFile " << fullPath << " to " << fullDestPath << std::endl;
	try {
	  fs::copy_file(fullPath, fullDestPath, fs::copy_option::overwrite_if_exists);
	}
	catch(boost::filesystem::filesystem_error e){
	  std::cerr << e.what() << std::endl;
	}
	return true;
}

bool LocalSyncManager::pushDir(fs::path rootPath, fs::path fullPath) const{
	const fs::path fullDestPath = constructDestPath(rootPath, fullPath, destPath);
	fs::recursive_directory_iterator pathIt(fullPath, fs::symlink_option::recurse);
  	fs::recursive_directory_iterator pathEnd;

  	mkdir(rootPath, fullPath);

 	while(pathIt != pathEnd){
    	 	const fs::path currentPath = (fs::path)*pathIt;
    
   		if(fs::is_directory(currentPath)){
      			mkdir(rootPath, currentPath);
     
    		}
    		else if(fs::is_symlink(currentPath)){
      			mkdir(rootPath, currentPath);

  		}
    		else if(fs::is_regular_file(currentPath)){
      			pushFile(rootPath, currentPath);

    		}
    		pathIt++;

  	}
	std::cout << "pushDir " << fullPath << " to " << fullDestPath << std::endl;
	return true;
}

bool LocalSyncManager::mkdir(fs::path rootPath, fs::path fullPath) const{
	fs::path fullDestPath = constructDestPath(rootPath, fullPath, destPath);
	std::cout << "mkdir " << fullPath << " on " << fullDestPath << std::endl;
	if(!fs::exists(fullDestPath)){
		fs::copy_directory(fullPath, fullDestPath);
	}
	return true;
}

bool LocalSyncManager::removeFile(fs::path rootPath, fs::path fullPath) const{
	fs::path fullDestPath = constructDestPath(rootPath, fullPath, destPath);
	std::cout << "removeFile " << fullPath << " from " << fullDestPath << std::endl;
	return fs::remove_all(fullDestPath);
}

bool LocalSyncManager::removeDir(fs::path rootPath, fs::path fullPath) const{
	fs::path fullDestPath = constructDestPath(rootPath, fullPath, destPath);
	std::cout << "removeDir " << fullPath << " from " << fullDestPath << std::endl;
	return fs::remove_all(fullDestPath);
}

fs::path constructDestPath(const fs::path rootPath, fs::path fullPath, fs::path destRootPath){
	return destRootPath.string() + fullPath.string().substr(rootPath.string().length(), fullPath.string().length());
}
