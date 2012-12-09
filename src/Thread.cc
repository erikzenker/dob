#include "Thread.h"

using namespace std;

Thread::Thread() {}

int Thread::Start(void* arg)
{
  SetArg(arg);
  int code = pthread_create( &mThreadId, NULL, Thread::EntryPoint, (void*) this);
  return code;
    
}

int Thread::Stop()
{
  return pthread_cancel(mThreadId);

}

int Thread::Suspend(){

}

int Thread::Resume(){

}

void Thread::Run(void* arg)
{
  Setup();
  Execute(arg);

}

/*static */
void * Thread::EntryPoint(void *pthis)
{
  Thread *pt = (Thread*)pthis;
  pt->Run(pt->GetArg());
}

void Thread::Setup()
{
  // Do any setup here
}

void Thread::SetArg(void* arg){
  this->mArg = arg;
}

void* Thread::GetArg(){
  return this->mArg;
}

void Thread::Execute(void* arg)
{

}
