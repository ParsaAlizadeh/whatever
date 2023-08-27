#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "vector.h"

#define PIPE "|"
#define PIPENCODE "\x1f"

extern vector *scan_line(FILE *inp);
extern vector *scan_strline(const char *inp);
extern void prettify(FILE *file, FILE *out);

#endif