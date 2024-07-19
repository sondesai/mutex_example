#ifndef LOG_H 
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

void log_format(const char* tag,const char* message, va_list args) {
   time_t now;
   //time(&now);
   now = time(NULL);
   printf("%ld:%s:",now,tag);
   vprintf(message, args);
}

void log_err(const char* message, ...) {
	va_list args;   
	va_start(args, message);
	log_format("error", message, args);
	va_end(args); 
}

void log_warn(const char* message, ...) {
	va_list args;   
	va_start(args, message);
	log_format("warning", message, args);
	va_end(args); 
}

void log_info(const char* message, ...) {
	va_list args;   
	va_start(args, message);
	log_format("info", message, args);
	va_end(args); 
}
#endif
