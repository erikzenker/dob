#include <dbg_print.h>


void dbg_print(int level, const char* format,...){
  if(level <= dbg_print_level){
    va_list arg;
    va_start (arg, format);
    vfprintf (stderr, format, arg);
    va_end (arg);

  }
  
}

void dbg_printc(int level, std::string className, std::string functionName, const char* format,...){
  if(level <= dbg_print_level){
    va_list arg;
    std::string finalFormat = "C ";
    switch(level){
    case LOG_FATAL:
      finalFormat.append("\033[31;1;5m[Fatal Error] ");
      break;
    case LOG_ERR:
      finalFormat.append("\033[31;1m[Error] ");
      break;
    case LOG_WARN:
      finalFormat.append("\033[33;1m[Warning] ");
      break;
    case LOG_INFO:
      finalFormat.append("\033[32;1m[Info] ");
      break;
    case LOG_DBG:
      finalFormat.append("\033[34;1m[Debug] ");
      break;
    default:
      break;
    }
    finalFormat.append("\033[m");
    if(dbg_print_level >= LOG_DBG){
      if(className.compare("")){
	finalFormat.append("\033[37m");
	finalFormat.append(className);
	finalFormat.append("\033[34m::");
      }
      finalFormat.append("\033[37m");
      finalFormat.append(functionName);
      finalFormat.append("\033[34m: \033[m");
    }
    finalFormat.append(format);
    finalFormat.append("\n");

    va_start (arg, format);
    vfprintf (stderr, finalFormat.c_str(), arg);
    va_end (arg);

  }

}
