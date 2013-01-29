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

// phrase_parse includes
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/bind.hpp>

#include <ProfileManager.h>
#include <dbg_print.h>

typedef sigc::signal<int> StopSignal;
typedef sigc::signal<int> StartSignal;


/**
 * @brief Manages communication between daemon
 *        and client. 
 *
 * The IPC problem is solved by linux fifo-pipes. This
 * are simple files where you can easily write on
 * and read from. By commands like start, stop
 * or restart you can change the state of profiles.
 * An easy bash daemon could use this interface to
 * control the backup client. Signals from sigc++
 * are used to propagate commands into the 
 * syncronisation process.
 *
 * Commands : * echo start profilename > fifo-file
 *            * echo stop profilename > fifo-file
 *            * echo restart profilename > fifo-file
 * 
 * @class InterProcessCommunication
 *        InterProcessCommunication.h
 *        "include/InterProcessCommunication.h"
 *
 **/
class InterProcessCommunication {
 public: 
  InterProcessCommunication(std::string pathname);
  ~InterProcessCommunication();
  sigc::signal<bool, std::string> getStopSignal();
  sigc::signal<bool, std::string> getStartSignal();
  sigc::signal<bool, std::string> getRestartSignal();

  bool read();

 private:
  int mFdFifo;
  char mBuf[100];
  void emitStopSignal(std::vector<char> profileName);
  void emitStartSignal(std::vector<char> profileName);
  void emitRestartSignal(std::vector<char> profileName);
  sigc::signal<bool, std::string> mStopSignal;
  sigc::signal<bool, std::string> mStartSignal;
  sigc::signal<bool, std::string> mRestartSignal;
};

#endif /* InterProcessCommunication_H */
