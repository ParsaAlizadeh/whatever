#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <stdio.h>

FILE *clipboard_reset(void);
FILE *clipboard_get(void);

// void clipboard_set(char *);
// const char *clipboard_get(void);

#endif