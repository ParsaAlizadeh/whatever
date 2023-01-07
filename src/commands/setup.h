#ifndef SETUP_H
#define SETUP_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"
#include "../vecstr.h"
#include "../vector.h"
#include "../command.h"

int cmdlog(const command *, const char *, ...);
int cmdlogrequired(const command *, char);

#endif