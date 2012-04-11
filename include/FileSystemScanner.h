#include <string>

using namespace std;

class FileSystemScanner {

 protected:
  string scan_folder;

 public:
  FileSystemScanner(string scan_folder);
  ~FileSystemScanner();
  string GetScanFolder();
  virtual int StartToScan() =0;
  virtual int StopToScan() =0;

};

