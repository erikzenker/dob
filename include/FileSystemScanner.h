/** 
 * @file      FileSystemScanner.h
 * @author    Erik Zenker
 * @date      28.04.2012
 * @copyright Gnu Public License
 */

#pragma once
#include <string>
#include <boost/filesystem.hpp>
#include <EventManager.h>


/**
 * @brief Watches a folder for changes. 
 * @class FileSystemScanner 
 *        FileSystemScanner.h 
 *        "include/FileSystemScanner.h"
 *
 * Watches a folder (scanFolder) for changes and submits the 
 * changes as event to an EventManager (pEventManager).
 *
 **/
class FileSystemScanner {
public:
  FileSystemScanner(const boost::filesystem::path rootPath, const EventManager& eventManager);
  ~FileSystemScanner();
  virtual void start() = 0;
  virtual void stop()  = 0;

protected:
  const boost::filesystem::path rootPath;
  const EventManager& eventManager;


};

