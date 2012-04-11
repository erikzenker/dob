#include <vector>
#include <iostream>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <stdlib.h>


using namespace std;

class EventManager{
private:
  vector<inotify_event*> mEventList;
  void HandleEvent(inotify_event* pEvent);
public:
  void PushBackEvent(inotify_event* pNewEvent);
};

