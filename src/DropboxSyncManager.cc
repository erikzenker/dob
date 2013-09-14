#include "DropboxSyncManager.h"

/*
 * @note  Rsync command line parameters
 *        -r, --recursive         recurse into directories
 *        -u, --update            skip files that are newer on the receiver
 *        -z, --compress          compress file data during the transfer
 *        -v, --verbose           increase verbosity
 *            --delete            delete extraneous files from dest dirs
 *        -L, --copy-links        transform symlink into referent file/dir
 *        -K, --keep-dirlinks     treat symlinked dir on receiver as dir
 *        -p, --perms             preserve permissions
 *        -t, --times             preserve modification times
 *        -o, --owner             preserve owner (super-user only)
 *        -g, --group             preserve group
 *            --devices           preserve device files (super-user only)
 *            --specials          preserve special files
 *        -D                      same as --devices --specials
 *            --modify-window=NUM compare mod-times with reduced accuracy
 *        See more in "man rsync"
 */
DropboxSyncManager::DropboxSyncManager( std::string destFolder, SyncType syncType):
  SyncManager(destFolder, syncType),
  mOauthKey("1o3430he5dxxgu8"),
  mOauthSecret("thbjxy0k1e4bau7"),
  mToken(""),
  mSecretToken(""),
  mAccesToken(""),
  mSecretAccesToken(""),
  mRequestUrl("https://api.dropbox.com/1/oauth/request_token")
  {

    setupDestination();
    
}


bool DropboxSyncManager::syncSourceFolder(std::string sourceFolder){
  dbg_printc(LOG_DBG, "DropboxSyncManager","SyncSourceFolder", "Syncronise source and destination folder");
  // std::string rsync_push_query = mAllOptionsPush;
  // std::string rsync_pull_query = mAllOptionsPull;

  // rsync_push_query
  //   .append(sourceFolder)
  //   .append(" ")
  //   .append(mDestUser)
  //   .append("@")
  //   .append(mDestHost)
  //   .append(":")
  //   .append(mDestFolder);

  // rsync_pull_query
  //   .append(mDestUser)
  //   .append("@")
  //   .append(mDestHost)
  //   .append(":")
  //   .append(mDestFolder)
  //   .append(" ")
  //   .append(sourceFolder);

  // dbg_printc(LOG_DBG, "DropboxSyncManager", "SyncSourceFolder", "Rsync pull string: %s", rsync_pull_query.c_str());
  // cerr << "\n";
  // system(rsync_pull_query.c_str());
  // dbg_printc(LOG_DBG, "DropboxSyncManager", "SyncSourceFolder", "Rsync push string: %s", rsync_push_query.c_str());
  // system(rsync_push_query.c_str());

  // return true;
  return false;
}

bool DropboxSyncManager::syncFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  // std::string rsync_query = mAllOptionsPush; 
  // rsync_query
  //   .append(syncFolder)
  //   .append(folder)
  //   .append(" ")
  //   .append(mDestUser)
  //   .append("@")
  //   .append(mDestHost)
  //   .append(":")
  //   .append(mDestFolder)
  //   .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  // dbg_printc(LOG_DBG,"DropboxSyncManager","SyncFolder","%s ", rsync_query.c_str());  
  // system(rsync_query.c_str());

  // return true;
  return false;
  
}

bool DropboxSyncManager::syncFile(std::string sourceFolder, std::string syncFolder){
  // std::string rsync_query = mAllOptionsPush;
  // rsync_query
  //   .append(syncFolder)
  //   //.append(folder)
  //   .append(" ")
  //   .append(mDestUser)
  //   .append("@")
  //   .append(mDestHost)
  //   .append(":")
  //   .append(mDestFolder)
  //   .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  // dbg_printc(LOG_DBG,"DropboxSyncManager","SyncFile","%s ", rsync_query.c_str());  
  // system(rsync_query.c_str());

  // return true;
  return false;
}

/**
 * @todo there should be a quicker alternative to remove files as rsync
 *
 **/
bool DropboxSyncManager::removeFolder(std::string sourceFolder, std::string syncFolder, std::string folder){
  // std::string rm_query = mAllOptionsPush; 
  // rm_query
  //   .append("--delete ")
  //   .append(syncFolder)
  //   .append(" ")
  //   .append(mDestUser)
  //   .append("@")
  //   .append(mDestHost)
  //   .append(":")
  //   .append(mDestFolder)
  //   .append(syncFolder.substr(sourceFolder.length(), syncFolder.length()));

  // dbg_printc(LOG_DBG, "DropboxSyncManager","RemoveFolder","%s", rm_query.c_str());
  // system(rm_query.c_str());

  // return true;
  return false;

}

/**
 * @todo fill with content
 *
 **/
bool DropboxSyncManager::checkDestination(){
  return false;
}

/**
 * @todo fill with content
 *
 **/
bool DropboxSyncManager::setupDestination(){
  // Build curl string
  dbg_printc(LOG_DBG, "DropboxSyncManager", "setupDestination", "Connect to dropbox server");
  std::string curlQuery = "curl --header \"Authorization: OAuth oauth_version=1.0, oauth_signature_method=PLAINTEXT ,oauth_consumer_key=";
  curlQuery
    .append(mOauthKey)
    .append(", oauth_signature=")
    .append(mOauthSecret)
    .append("&\" ")
    .append(mRequestUrl);

  // Call curl
  FILE *fd = popen(curlQuery.c_str(), "r");
  if(fd <= 0){
    dbg_printc(LOG_DBG, "DropboxSyncManager", "setupDestination", "Failed to connect to dropbox server");
  }

  // Parse output
  namespace qi = boost::spirit::qi;
  char buf[100];
  fgets(buf, sizeof(buf)-1, fd);
  dbg_printc(LOG_DBG, "DropboxSyncManager", "setupDestination", "%s", buf);
  std::string line(buf);

  std::vector<char> v;

  qi::phrase_parse(line.begin(), line.end(), "oauth_token_secret=" >> *qi::char_("0-9a-z"), qi::space);

  std::string token(v.begin(), v.end());

  std::cerr << token << "\n";

  return true;
}
