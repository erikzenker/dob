/**
 * @file      InotifyFileSystemScanner.h
 * @author    Erik Zenker
 * @date      31.05.2012
 * @copyright Gnu Public License
 **/

#ifndef InotifyFileSystemScanner_H
#define InotifyFileSystemScanner_H

#include <string>
#include <vector>
#include <Thread.h>
#include <FileSystemScanner.h>
#include <Inotify.h>

/**
 * @brief Watches a folder for changes with the help of
 *        Inotify mechanism of linux kernel.
 *
 * @class InotifyFileSystemScanner 
 *        InotifyFileSystemScanner.h
 *        "include/InotifyFileSystemScanner.h"
 *
 * Watches a folder (scanFolder) for changes. Every change
 * will raise an event of Inotify which will be processed
 * by a Eventmanager (pEventManager). New folders or files will 
 * also watched in future.
 *
 **/
class InotifyFileSystemScanner : public FileSystemScanner, public Thread {
public:
  InotifyFileSystemScanner(const std::string scanFolder, std::vector<std::string> ignoredFolders, const int eventTimeout, EventManager* const pEventManager);
  ~InotifyFileSystemScanner();
  virtual int startToScan();
  virtual int stopToScan();

 protected:
  virtual void execute(void* arg);
  virtual void setup();

 private:
  Inotify mInotify;

};

#endif /* InotifyFileSystemScanner_H */
