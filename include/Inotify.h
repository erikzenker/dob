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
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>

#include <string>
#include <cstring>
#include <queue>
#include <map>
#include <dbg_print.h>
#include <FileSystemEvent.h>

#define MAX_EVENTS     4096
#define EVENT_SIZE     (sizeof (inotify_event))
#define EVENT_BUF_LEN  (MAX_EVENTS * (EVENT_SIZE + 16))

class Inotify {
 public:
  Inotify();
  ~Inotify();
  bool WatchFolderRecursively(std::string watchFolder);
  FileSystemEvent<int>* GetNextEvent();
  int GetLastError();
  std::string WdToFilename(int wd);
  
 private:
  bool IsDir(std::string folder);
  bool WatchFile(std::string file);
  bool Initialize();
  bool CleanUp();
  std::string EventMaskToString(uint32_t events);

  // Member
  bool mIsInitialized;
  int mInotifyFd;
  int mError;
  uint32_t mEventMask;
  //std::queue<inotify_event*> mEventQueue;
  std::queue<FileSystemEvent<int>* > mEventQueue;
  std::map<int, std::string> mFolderMap;

};

#endif /* INOTIFY_H */
