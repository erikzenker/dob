#include "LocalSyncManager.h"

LocalSyncManager::LocalSyncManager(string destFolder, string syncType):
  SyncManager(destFolder, syncType){

}

bool LocalSyncManager::CheckDestFolder(){
  static struct stat64 my_stat;
  if ( -1 == lstat64(mDestFolder.c_str(), &my_stat ) ) {
    if (errno == ENOENT) return false;
    dbg_printc(LOG_WARN, "LocalSyncManager", "IsDir", "\nC Stat failed on %s: %s\n", mDestFolder.c_str(), strerror(errno));
    return false;
  }
  if(S_ISDIR( my_stat.st_mode ))
    return true;
  return false;
}

bool LocalSyncManager::MountDestFolder(){
  return false;
}
