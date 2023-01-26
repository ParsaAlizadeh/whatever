#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

int vloginfo(const char *format, va_list ap);
int loginfo(const char *format, ...);

#endif
