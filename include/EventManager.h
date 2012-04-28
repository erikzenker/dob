#include <vector>
#include <iostream>
#include <string>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>
#include <stdlib.h>
#include "RemoteSyncManager.h"

using namespace std;

class EventManager{
private:
  vector<inotify_event*> mEventList;
  SyncManager* mpSyncManager;


  void HandleEvent(inotify_event* pEvent, string sourceFolder);
public:
  EventManager(SyncManager * pSyncManager);
  void PushBackEvent(inotify_event* pNewEvent, string sourceFolder);
};

