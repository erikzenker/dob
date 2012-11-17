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
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::EmitStopSignal, this, _1)]
		     ,
		     space);

    qi::phrase_parse(readString.begin(), readString.end()
		     ,  
		     qi::string("start ") 
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::EmitStartSignal, this, _1)]
		     ,
		     space);


    qi::phrase_parse(readString.begin(), readString.end()
		     ,  
		     qi::string("restart ") 
		     >> (*qi::char_("a-zA-Z"))[boost::bind(&InterProcessCommunication::EmitRestartSignal, this, _1)]
		     ,
		     space);



  }
  return true;
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

void InterProcessCommunication::EmitStopSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStopSignal.emit(name);
}

void InterProcessCommunication::EmitStartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mStartSignal.emit(name);
}

void InterProcessCommunication::EmitRestartSignal(std::vector<char> profileName){
  string name(profileName.begin(), profileName.end());
  mRestartSignal.emit(name);
}
