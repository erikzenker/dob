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

  memset(&mBuf, 0, BUF_SIZE);

}

  
InterProcessCommunication::~InterProcessCommunication(){
  close(mFdFifo);
}

// TODO Check if fifo was created correctly
/**
 * @brief Starts to read from fifo pipe
 *
 * The read on the pipe will be blocking
 * and loop forever. So eather you have no
 * problem by this or you need to put it
 * into an own thread.
 **/
bool InterProcessCommunication::readFromPipe(){
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  using ascii::space;

  while(1){
    if(read(mFdFifo, &mBuf, sizeof(mBuf)) < 0){
      mError = errno;
      dbg_printc(LOG_ERR, "InterProcessCommunication", "readFromPipe", "Failed to read from fifo pipe(%d), Errno %d",mFdFifo, mError);
    }
    dbg_printc(LOG_DBG, "InterProcessCommunication", "readFromPipe","%s",mBuf);
    std::string readString(mBuf);
    std::string profileName = "default";

    qi::phrase_parse(readString.begin(), readString.end()
		     ,  
		     qi::string("stop ") 
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::emitStopSignal, this, _1)]
		     ,
		     space);

    qi::phrase_parse(readString.begin(), readString.end()
		     ,  
		     qi::string("start ") 
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::emitStartSignal, this, _1)]
		     ,
		     space);


    qi::phrase_parse(readString.begin(), readString.end()
		     ,  
		     qi::string("restart ") 
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::emitRestartSignal, this, _1)]
		     ,
		     space);


    memset(&mBuf, 0, BUF_SIZE);
  }
  return true;
}

/**
 * @brief  The returned signal can be used to connect it to a
 *         signalhandler function. The Signal will be called,
 *         by signal.emit()
 *
 * @return Stopsignal 
 **/
sigc::signal<bool, std::string> InterProcessCommunication::getStopSignal(){
  return mStopSignal;
}

/**
 * @brief  The returned signal can be used to connect it to a
 *         signalhandler function. The Signal will be called,
 *         by signal.emit()
 *
 * @return Startsignal 
 **/
sigc::signal<bool, std::string> InterProcessCommunication::getStartSignal(){
  return mStartSignal;
}

/**
 * @brief  The returned signal can be used to connect it to a
 *         signalhandler function. The Signal will be called,
 *         by signal.emit()
 *
 * @return Restartsignal 
 **/
sigc::signal<bool, std::string> InterProcessCommunication::getRestartSignal(){
  return mRestartSignal;
}

/**
 * @brief Emits signals to all connected signalhandler of signal mStopsignal
 *  
 * @param profile, that will be stopped
 **/
void InterProcessCommunication::emitStopSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStopSignal.emit(name);
}

/**
 * @brief Emits signals to all connected signalhandler of signal mStopsignal
 *
 * @param profile, that will be started
 **/

void InterProcessCommunication::emitStartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStartSignal.emit(name);
}

/**
 * @brief Emits signals to all connected signalhandler of signal mStopsignal
 *
 * @param profile, that will be restarted
 **/
void InterProcessCommunication::emitRestartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mRestartSignal.emit(name);
}
