#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

class Thread
{
public:
  Thread();
  int Start(void* arg);
  int Stop();
  int Suspend();
  int Resume();

protected:
  void Run(void* arg);
  static void* EntryPoint(void*);
  virtual void Setup();
  virtual void Execute(void* arg);
  void SetArg(void* arg);
  void* GetArg();
private:
  pthread_t mThreadId;
  void* mArg;

};

#endif /* THREAD_H */
