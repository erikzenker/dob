/** 
 * @file      Inotify.h
 * @author    Erik Zenker
 * @date      02.11.2012
 * @copyright Gnu Public License
 **/
#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/inotify.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> // DIR, dirent
#include <sys/stat.h> // lstat64
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string>
#include <cstring> // strcmp
#include <queue>
#include <regex>
#include <boost/filesystem.hpp> // boost::filesystem

#include <map>
#include <dbg_print.h>
#include <FileSystemEvent.h>

#define MAX_EVENTS     4096
#define EVENT_SIZE     (sizeof (inotify_event))
#define EVENT_BUF_LEN  (MAX_EVENTS * (EVENT_SIZE + 16))

/**
 * @brief C++ wrapper for linux inotify interface
 * @class Inotify
 *        Inotify.h
 *        "include/Inotify.h"
 *
 * folders will be watched by watchFolderRecursively or
 * files by watchFile. If there are changes inside this
 * folder or files events will be raised. This events
 * can be get by getNextEvent.
 **/
class Inotify {
 public:
  Inotify(std::vector<std::string> ignoredFolders, int eventTimeout);
  ~Inotify();
  bool watchFolderRecursively(std::string watchFolder);
  bool watchFile(boost::filesystem::path file);
  FileSystemEvent<int>* getNextEvent();
  int getLastError();
  std::string wdToFilename(int wd);
  bool removeWatch(int wd);
  
 private:
  bool isDir(std::string folder);
  bool initialize();
  bool isIgnored(std::string file);
  void clearEventQueue();
  bool onTimeout(time_t eventTime);
  bool checkEvent(FileSystemEvent<int>* event);

  // Member
  int mError;
  time_t mEventTimeout;
  time_t mLastEventTime;
  uint32_t mEventMask;
  std::vector<std::string> mIgnoredFolders;
  std::queue<FileSystemEvent<int>* > mEventQueue;
  std::map<int, std::string> mFolderMap;
  bool mIsInitialized;
  int mInotifyFd;


};

#endif /* INOTIFY_H */
