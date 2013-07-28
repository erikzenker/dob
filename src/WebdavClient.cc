#include <string>
#include <exception> /* exception */
#include <unistd.h> /* open, close */
#include <stdlib.h>
#include <fcntl.h> /* open, close */

#include <WebdavClient.h>
#include <WebdavPath.h>
#include <neon/ne_props.h> /* ne_simple_propfind, ne_prop_result_set */
#include <neon/ne_uri.h> /* uri */
#include <dbg_print.h>

// @TODO replace or tolerate "http://" in url string
WebdavClient::WebdavClient(std::string url, std::string user, std::string pass){
  ne_sock_init();
  mSession = ne_session_create("http", url.c_str(), 80);

}

WebdavClient::~WebdavClient(){
  ne_session_destroy(mSession);
  ne_sock_exit();
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


