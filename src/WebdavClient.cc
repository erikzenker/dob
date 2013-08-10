#include <string>
#include <unistd.h> /* open, close */
#include <stdlib.h>
#include <fcntl.h> /* open, close */
#include <cstring> /* strncpy */
#include <iostream> /* std::cout */

#include <WebdavClient.h>
#include <WebdavPath.h>
#include <neon/ne_props.h> /* ne_simple_propfind, ne_prop_result_set */
#include <neon/ne_uri.h> /* uri */
#include <dbg_print.h>
#include <neon/ne_auth.h> /* ne_set_server_auth, ne_ssl_trust_default_ca */


// @TODO replace or tolerate "http://" in url string
WebdavClient::WebdavClient(std::string url, std::string user, std::string pw){
  ne_sock_init();
  mSession = ne_session_create("http", url.c_str(), 80);
  std::vector<std::string> *login = new std::vector<std::string>();
  login->push_back(user);
  login->push_back(pw);
  ne_set_server_auth(mSession, WebdavClient::defineAuth, login);
  // Is mentioned in neon webdac example
  // but runs in seg fault. Works also
  // without this function.
  // ne_ssl_trust_default_ca(mSession);

}

WebdavClient::~WebdavClient(){
  ne_forget_auth(mSession);
  ne_session_destroy(mSession);
  ne_sock_exit();
}

int WebdavClient::defineAuth(void *userdata, const char *realm, int attempts, char *username, char *password) { 
  std::vector<std::string> *login = (std::vector<std::string>*) userdata;
  strncpy(username, login->at(0).c_str(), NE_ABUFSIZ); 
  strncpy(password, login->at(1).c_str(), NE_ABUFSIZ); 
  return attempts; 
}

/**
 * @brief Checks for existance of file/diretory at
 *        given uri. This can be used to reduce transfer 
 *        bandwidth, so no data need to be transfered 
 *        which allready exists.
 *
 * @param uri of file/directory on webdav-server
 *
 * @return true if file/directory exists
 *         false if file/directory not exists
 *
 **/
bool WebdavClient::exists(std::string uri){
  dbg_printc(LOG_DBG, "WebdavClient", "exists", "Test directory/file for existance: %s", uri.c_str());
  const int depth = NE_DEPTH_ZERO; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
  std::vector<WebdavPath> * props = new std::vector<WebdavPath>;
   int res = ne_simple_propfind(mSession, uri.c_str(), depth, NULL, getProps, props);
   if(res!=NE_OK){ 
     std::string error = ne_get_error(mSession);
     if(error.find("404")!= std::string::npos)
       return false;
     return true;

   } 
   props->erase(props->begin());
   return true;

}

std::vector<WebdavPath> WebdavClient::ls(std::string uri){
  dbg_printc(LOG_DBG, "WebdavClient", "ls", "List directory: %s", uri.c_str());
  const int depth = NE_DEPTH_ONE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
  std::vector<WebdavPath> * props = new std::vector<WebdavPath>;
   int res = ne_simple_propfind(mSession, uri.c_str(), depth, NULL, getProps, props);
   if(res!=NE_OK){ 
     dbg_printc(LOG_WARN,"WebdavClient", "ls","Propfind-request failed: %s", ne_get_error(mSession)); 
     return *props;
   } 
   props->erase(props->begin());
   return *props;
}

void WebdavClient::getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set){

  std::vector<WebdavPath> *paths = (std::vector<WebdavPath> *) userdata;

  ne_propname props[] = {
    { "DAV:", "resourcetype" }, 
    { "DAV:", "getlastmodified" },
    { "DAV:", "getcontenttype"}
  };
  dbg_printc(LOG_DBG, "WebdavClient", "getProps", "%s %s %s %s %s", uri->host, uri->path, ne_propset_value(set, &props[0]), ne_propset_value(set, &props[1]), ne_propset_value(set, &props[2]));

  std::string ressourceType = ne_propset_value(set, &props[0]) ? ne_propset_value(set, &props[0]) : std::string();
  std::string lastModified  =  ne_propset_value(set, &props[1]) ?  ne_propset_value(set, &props[1]) : std::string();
  std::string contentType   = ne_propset_value(set, &props[2]) ?  ne_propset_value(set, &props[2]) : std::string();


  WebdavPath path(uri->host,
		  uri->path,
		  ressourceType,
		  lastModified,
		  contentType);

  // Push information to userdata
  paths->push_back(path);

}

bool WebdavClient::put(std::string uri, std::string localSource){
  int fd = open(localSource.c_str(), O_RDONLY); 
  int res = ne_put(mSession, uri.c_str(), fd); 
  if(res!=NE_OK){ 
    dbg_printc(LOG_WARN,"WebdavClient", "put","Put-request failed: %s", ne_get_error(mSession)); 
    return false;
  } 
  close(fd);
  return true;
}

bool WebdavClient::get(std::string uri, std::string localDestination){
  int fd = open(localDestination.c_str(), O_WRONLY | O_CREAT); 
  int res = ne_get(mSession, uri.c_str(), fd); 
  if(res!=NE_OK){ 
    dbg_printc(LOG_WARN,"WebdavClient", "get","Get-request failed: %s", ne_get_error(mSession)); 
    return false;
  } 
  close(fd);
  return true;

}

bool WebdavClient::mkdir(std::string uri){
  int res = ne_mkcol(mSession, uri.c_str());
  if(res != NE_OK){ 
    dbg_printc(LOG_WARN,"WebdavClient", "mkol","Mkol-request failed: %s", ne_get_error(mSession)); 
    return false;
  }
  return true;
}

bool WebdavClient::del(std::string uri){
  int res = ne_delete(mSession, uri.c_str());
  if(res!=NE_OK){
    dbg_printc(LOG_WARN,"WebdavClient", "delete","Delete-request failed: %s", ne_get_error(mSession)); 
    return false;
  }
  return true;
}


