#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

/**
 * @brief Interface to add thread support to a class
 * @class Thread
 *        Thread.h
 *        "include/Thread.h"
 * 
 * If your class inherites from Thread class it is able
 * to run in a different thread. The code you want to
 * execute should be in the Execute() method or called from there
 * and you can use Setup() to prepare your class for the thread.
 * With Start() you start and with Stop() you can terminate the thread.
 * Suspend() and Resume() is not implemented yet.
 *
 **/
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
  virtual void Setup() = 0;
  virtual void Execute(void* arg) = 0;
  void SetArg(void* arg);
  void* GetArg();
private:
  pthread_t mThreadId;
  void* mArg;

};

#endif /* THREAD_H */
