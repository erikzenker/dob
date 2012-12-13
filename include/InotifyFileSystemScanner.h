/**
 * @file      InotifyFileSystemScanner.h
 * @author    Erik Zenker
 * @date      31.05.2012
 * @copyright Gnu Public License
 **/

#ifndef InotifyFileSystemScanner_H
#define InotifyFileSystemScanner_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <sys/inotify.h>
#include <assert.h>
#include <dirent.h>

#include <Thread.h>
#include <FileSystemScanner.h>
#include <dbg_print.h>
#include <Inotify.h>
#include <FileSystemEvent.h>
#include <Profile.h>

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
  InotifyFileSystemScanner(const string scanFolder, EventManager* const pEventManager);
  InotifyFileSystemScanner(const string scanFolder, const string ignoredFolder, const int eventTimeout, EventManager* const pEventManager);
  virtual int StartToScan();
  virtual int StopToScan();

 protected:
  virtual void Execute(void* arg);
  virtual void Setup();

 private:
  // Member
  std::string mIgnoredFolder;
  int mEventTimeout;


};

#endif /* InotifyFileSystemScanner_H */
