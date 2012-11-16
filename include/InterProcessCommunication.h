/**
 * @file      InterProcessCommunication.h
 * @author    Erik Zenker
 * @date      11.11.2012
 * @copyright Gnu Public License
 **/
#ifndef InterProcessCommunication_H
#define InterProcessCommunication_H

#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sigc++/sigc++.h>

#include <dbg_print.h>

/**
 * @brief Manages communication between daemon
 *        and client.
 * 
 * @class InterProcessCommunication
 *        InterProcessCommunication.h
 *        "include/InterProcessCommunication.h"
 *
 * If the programm runs in nogui mode then the
 * user needs a possibility to control functions
 * like stop, start of the syncprocess. This can
 * be handled though ipc. An easy way for ipc on
 * Linux machines are named pipes(fifos). Maybe
 * you could also abstract this class a bit further,
 * but now there is no need for that.
 *
 **/
typedef sigc::signal<int> StopSignal;
typedef sigc::signal<int> StartSignal;


class InterProcessCommunication {
 public: 
  InterProcessCommunication(std::string pathname);
  ~InterProcessCommunication();
  sigc::signal<bool, std::string> GetStopSignal();
  sigc::signal<bool, std::string> GetStartSignal();
  sigc::signal<bool, std::string> GetRestartSignal();

  bool Read();

 private:
  int mFdFifo;
  char mBuf[100];
  sigc::signal<bool, std::string> mStopSignal;
  sigc::signal<bool, std::string> mStartSignal;
  sigc::signal<bool, std::string> mRestartSignal;
};

#endif /* InterProcessCommunication_H */
