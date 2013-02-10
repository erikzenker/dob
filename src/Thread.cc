#include "Thread.h"

using namespace std;

Thread::Thread(){

}

Thread::~Thread(){
  free(mArg);
}

int Thread::start(void* arg)
{
  setArg(arg);
  int code = pthread_create( &mThreadId, NULL, Thread::entryPoint, (void*) this);
  return code;
    
}

int Thread::stop()
{
  return pthread_cancel(mThreadId);

}

int Thread::suspend(){

  return 0;
}

int Thread::resume(){

  return 0;
}

void Thread::run(void* arg)
{
  setup();
  execute(arg);

}

/*static */
void * Thread::entryPoint(void *pthis)
{
  Thread *pt = (Thread*)pthis;
  pt->run(pt->getArg());
  return pt;
}

void Thread::setup()
{
  // Do any setup here
}

void Thread::setArg(void* arg){
  this->mArg = arg;
}

void* Thread::getArg(){
  return this->mArg;
}

void Thread::execute(void* arg)
{

}
