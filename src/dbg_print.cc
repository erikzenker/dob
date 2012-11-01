#include <dbg_print.h>

//extern "C" {
void dbg_print(int level, const char* format,...){
  if(level <= dbg_print_level){
    va_list arg;
    va_start (arg, format);
    vfprintf (stderr, format, arg);
    va_end (arg);

  }
  
}

//}
