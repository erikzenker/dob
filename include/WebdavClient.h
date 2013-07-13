#ifndef WebdavClient_H
#define WebdavClient_H

#include <string>
#include <vector>
#include "WebdavPath.h"
#include <neon/ne_basic.h> /* ne_session */
#include <neon/ne_uri.h> /* uri */
#include <neon/ne_props.h> /* ne_prop_result_set */

class WebdavClient {
public:
WebdavClient(std::string url, std::string user, std::string pass);
~WebdavClient();
std::vector<WebdavPath> ls(std::string uri);
bool put(std::string uri, std::string localSource);
bool get(std::string uri, std::string localDestination);
bool mkdir(std::string uri);
bool del(std::string uri);

private:
  static void getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set);
  std::string mUrl;
  std::string mUser;
  std::string mPass;
  ne_session *mSession;


};

#endif /* WebdavClient_H */
