#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <stdio.h>

extern FILE *clipboard_reset(void);
extern FILE *clipboard_get(void);

#endif
