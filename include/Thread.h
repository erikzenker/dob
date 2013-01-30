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
 * execute should be in the execute() method or called from there
 * and you can use setup() to prepare your class for the thread.
 * With start() you start and with stop() you can terminate the thread.
 * suspend() and resume() is not implemented yet.
 *
 **/
class Thread
{
public:
  Thread();
  ~Thread();
  int start(void* arg);
  int stop();
  int suspend();
  int resume();

protected:
  void run(void* arg);
  static void* entryPoint(void*);
  virtual void setup() = 0;
  virtual void execute(void* arg) = 0;
  void setArg(void* arg);
  void* getArg();
private:
  pthread_t mThreadId;
  void* mArg;

};

#endif /* THREAD_H */
