#include <string>
#include "EventManager.h"

using namespace std;

class FileSystemScanner {
protected:
  string mScanFolder;
  EventManager * mpEventManager;

public:
  FileSystemScanner(string scanFolder, EventManager* pEventManager);
  ~FileSystemScanner();
  string GetScanFolder();
  virtual int StartToScan() =0;
  virtual int StopToScan() =0;

};

