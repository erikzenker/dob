/* 
 * @file      dbg_print.h
 * @author    Erik Zenker
 * @date      01.11.2012
 * @copyright Gnu Public License
 */

#ifndef DBG_PRINT_H
#define DBG_PRINT_H

#include <stdio.h>
#include <stdarg.h>


// Defines
#define LOG_FATAL    (1) // red
#define LOG_ERR      (2) // red
#define LOG_WARN     (3) // yellow
#define LOG_INFO     (4) // green
#define LOG_DBG      (5) // white

extern int dbg_print_level;
void dbg_print(int level, const char* format,...);

#endif /* DBG_PRINT_H */
