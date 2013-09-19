#ifndef WebdavClient_H
#define WebdavClient_H

#include <string>
#include <vector>
#include <neon/ne_basic.h> /* ne_session */
#include <neon/ne_uri.h> /* uri */
#include <neon/ne_props.h> /* ne_prop_result_set */

struct WebdavPath {
WebdavPath(std::string host, std::string path, std::string ressourceType, std::string lastModified, std::string contentType) :
  host(host),
  path(path),
  ressourceType(ressourceType),
  lastModified(lastModified),
  contentType(contentType) {
  
  }
  
  bool isFile(){
    if(!ressourceType.compare(""))
      return true;
    return false;
  }

  bool isDirectory(){
    return !isFile();
  }

  std::string host;
  std::string path;
  std::string ressourceType;
  std::string lastModified;
  std::string contentType;

};


class WebdavClient {
 public:
  WebdavClient(std::string url, unsigned port, std::string user, std::string pass);
  ~WebdavClient();

  std::vector<WebdavPath> ls(std::string uri);
  std::vector<WebdavPath> tree(std::string uri);
  bool exists(std::string uri);
  bool put(std::string uri, std::string localSource);
  bool get(std::string uri, std::string localDestination);
  bool mkdir(std::string uri);
  bool rm(std::string uri);


 private:
  static int setLogin(void *userdata, const char *realm, int attempts, char *usernmae, char *password);
  static void getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set);
  std::string mUrl;
  std::string mUser;
  std::string mPass;
  ne_session *mSession;

};

#endif /* WebdavClient_H */
