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
}

bool InterProcessCommunication::read(){
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  using ascii::space;

  while(1){
    read(mFdFifo, &mBuf, sizeof(mBuf));
    dbg_printc(LOG_DBG, "InterProcessCommunication", "Read","%s",mBuf);
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



  }
  return true;
}

sigc::signal<bool, std::string> InterProcessCommunication::getStopSignal(){
  return mStopSignal;
}

sigc::signal<bool, std::string> InterProcessCommunication::getStartSignal(){
  return mStartSignal;
}

sigc::signal<bool, std::string> InterProcessCommunication::getRestartSignal(){
  return mRestartSignal;
}

void InterProcessCommunication::emitStopSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStopSignal.emit(name);
}

void InterProcessCommunication::emitStartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStartSignal.emit(name);
}

void InterProcessCommunication::emitRestartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mRestartSignal.emit(name);
}
