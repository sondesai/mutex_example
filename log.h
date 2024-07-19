#ifndef LOG_H 
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

void log_format(const char* tag,const char* message, va_list args);
void log_err(const char* message, ...);
void log_warn(const char* message, ...);
void log_info(const char* message, ...);


#endif
