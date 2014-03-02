#include <FileSystemScanner.h>

#include <boost/filesystem.hpp>
#include <exception>


FileSystemScanner::FileSystemScanner(const boost::filesystem::path rootPath, const EventManager& eventManager):
	rootPath(rootPath),
	eventManager(eventManager){

	if(!boost::filesystem::exists(rootPath)){
		throw std::invalid_argument("Can't initialize FileSystemscanner! RootPath does not exist. RootPath: " + rootPath.string());
	}
		
}

FileSystemScanner::~FileSystemScanner(){

}




