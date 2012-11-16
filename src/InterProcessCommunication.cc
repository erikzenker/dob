#include <InterProcessCommunication.h>

InterProcessCommunication::InterProcessCommunication(std::string pathname){
  if(mkfifo(pathname.c_str(), S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1){
    int error = errno;
    dbg_printc(LOG_ERR, "InterProcessCommunication","InterProcessCommunication","Couldn't make fifo, Errno %d", error);

  }

  mFdFifo = open(pathname.c_str(), O_RDWR);
  if(mFdFifo == -1){
    int error = errno;
    dbg_printc(LOG_ERR, "InterProcessCommunication","InterProcessCommunication","Couldn't open fifo, Errno %d", error);

  }

}

InterProcessCommunication::~InterProcessCommunication(){
  close(mFdFifo);
  free(mBuf);
}

bool InterProcessCommunication::Read(){
  while(1){
    read(mFdFifo, &mBuf, sizeof(mBuf));
    dbg_printc(LOG_DBG, "InterProcessCommunication", "Read","%s",mBuf);
    std::string readString(mBuf);

    if (readString.find("start") != std::string::npos)
       mStartSignal.emit("start_test");
    if (readString.find("stop") != std::string::npos)
       mStopSignal.emit("stop_test");
    if (readString.find("restart") != std::string::npos)
       mStopSignal.emit("restart_test");

  }
}

sigc::signal<bool, std::string> InterProcessCommunication::GetStopSignal(){
  return mStopSignal;
}

sigc::signal<bool, std::string> InterProcessCommunication::GetStartSignal(){
  return mStartSignal;
}

sigc::signal<bool, std::string> InterProcessCommunication::GetRestartSignal(){
  return mRestartSignal;
}
