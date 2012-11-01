/* 
 * @file      InotifyFileSystemScanner.h
 * @author    Erik Zenker
 * @date      31.05.2012
 * @copyright Gnu Public License
 */

#ifndef InotifyFileSystemScanner_H
#define InotifyFileSystemScanner_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <assert.h>
#include <dirent.h>

#include "Thread.h"
#include "FileSystemScanner.h"
#include "dbg_print.h"

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
 * by a Eventmanager (pEventManager). New folders or will 
 * be also added to the watches.
 *
 **/
class InotifyFileSystemScanner : public FileSystemScanner, public Thread {
public:
  InotifyFileSystemScanner(const string scanFolder, EventManager* const pEventManager);
  virtual int StartToScan();
  virtual int StopToScan();
 protected:
  virtual void Execute(void* arg);
  virtual void Setup();


};

#endif /* InotifyFileSystemScanner_H */
