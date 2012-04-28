#include <string>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

using namespace std;

class SyncManager{
protected:
  string mDestFolder;
public:
  SyncManager(string destFolder);
  SyncManager();
  void SyncSourceFolder(string sourceFolder);

};
