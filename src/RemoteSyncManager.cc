#include "RemoteSyncManager.h"

RemoteSyncManager::RemoteSyncManager(string destFolder, string syncType, string mountOptions):
  SyncManager(destFolder, syncType),
  mMountOptions(mountOptions){

}

bool RemoteSyncManager::CheckDestFolder(){
  string mountpoint_query = "mountpoint -q ";
  mountpoint_query.append(mDestFolder);
  return !system(mountpoint_query.c_str());
}

bool RemoteSyncManager::MountDestFolder(){
  if(!mMountOptions.compare(""))
    return false;

  string mount_query;
  mount_query
    .append("sshfs -o ServerAliveInterval=15 ")
    .append(mMountOptions)
    .append(" ")
    .append(mDestFolder);
  cerr << mount_query << "\n";
  system(mount_query.c_str());

  return true;
}
