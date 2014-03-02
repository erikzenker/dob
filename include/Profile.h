/**
 * @file      Profile.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 **/

#pragma once
#include <string>
#include <vector>
#include <FileSystemScanner.h>
#include <SyncManager.h>
#include <EventManager.h>
#include <SyncType.h>

struct ProfileData {
  	std::string profileName;
  	SyncType syncType;
  	std::string syncPath;
  	std::string syncProtocol;
  	std::string destUser;
  	std::string destHost;
  	std::string destPort;
  	std::string destPath;
  	std::string destPass;
  	std::vector<std::string> ignoredPaths;
};

/**
 * @brief Stores all information about a user profile
 *        and is able to start/stop the profile.
 *
 */
class Profile {
 public:
  Profile(FileSystemScanner& fileSystemScanner, EventManager& eventManager, SyncManager& syncManager);
  
  void start();
  void stop();

private:
  /* Member */
  FileSystemScanner&       fileSystemScanner;
  EventManager&            eventManager;
  SyncManager&             syncManager;

};


