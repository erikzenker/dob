/**
 * @file      InotifyFileSystemScanner.h
 * @author    Erik Zenker
 * @date      31.05.2012
 * @copyright Gnu Public License
 **/

#pragma once
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

#include <FileSystemScanner.h>
#include <FileStateDatabase.h>
#include <Inotify.h>

/**
 * @brief Watches a folder for changes with the help of
 *        Inotify mechanism of linux kernel.
 *
 * @class InotifyFileSystemScanner 
 *        InotifyFileSystemScanner.h
 *        "include/InotifyFileSystemScanner.h"
 *
 * Watches a folder (rootPath) for changes. Every change
 * will raise an event of Inotify which will be processed
 * by a Eventmanager (eventManager). New folders or files will 
 * also watched in future.
 *
 **/
class InotifyFileSystemScanner : public FileSystemScanner{
public:
  InotifyFileSystemScanner(const boost::filesystem::path rootPath, const EventManager& eventManager, const std::vector<std::string> ignoredFolders, const unsigned eventTimeout, const std::string profileName);
  ~InotifyFileSystemScanner();
  virtual void start();
  virtual void stop();

 private:
  FileSystemEvent toFileSystemEvent(std::pair<FileState, ModState> update);
  Inotify _inotify;
  FileStateDatabase _fileStateDatabase;

};

