#include <pthread.h>
#include <iostream>

class Thread
{
public:
  Thread();
  int Start(void* arg);
protected:
  int Run(void* arg);
  static void* EntryPoint(void*);
  virtual void Setup();
  virtual void Execute(void* arg);
  void SetArg(void* arg);
  void* GetArg();
private:
  pthread_t mThreadId;
  void* mArg;

};
