/** 
 * @file      FileSystemScanner.h
 * @author    Erik Zenker
 * @date      28.04.2012
 * @copyright Gnu Public License
 */

#ifndef FileSystemScanner_H
#define FileSystemScanner_H

#include <string>
#include "EventManager.h"

using namespace std;

/**
 * @brief Watches a folder for changes. 
 * @class FileSystemScanner 
 *        FileSystemScanner.h 
 *        "include/FileSystemScanner.h"
 *
 * Watches a folder (scanFolder) for changes and submits the 
 * changes as event to an EventManager (pEventManager).
 *
 */
class FileSystemScanner {
public:
  FileSystemScanner(const string scanFolder, EventManager* const pEventManager);
  ~FileSystemScanner();
  string GetScanFolder() const;
  EventManager* GetEventManager() const;
  virtual int StartToScan() = 0;
  virtual int StopToScan()  = 0;

protected:
  const string mScanFolder;
  EventManager* const mpEventManager;


};

#endif /* FileSystemScanner */
