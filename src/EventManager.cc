#include <EventManager.h>

void EventManager::PushBackEvent(inotify_event* pNewEvent){
  mEventList.push_back(pNewEvent);
  HandleEvent(pNewEvent);

}

void EventManager::HandleEvent(inotify_event* pEvent){
  switch(pEvent->mask){
  case IN_CREATE:
    cerr << "\nC Handle create event";
    break;
  case IN_DELETE:
    cerr << "\nC Handle delete event";
    break;
  case IN_MODIFY:
    cerr << "\nC Handle modify event";
    break;
  default:
    cerr << "\nC No handler for this event implementet";
    break;

  }
}


