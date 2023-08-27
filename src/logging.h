#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

extern int vloginfo(const char *format, va_list ap);
extern int loginfo(const char *format, ...);

#endif
