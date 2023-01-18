#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "vector.h"

#define PIPE "|"
#define PIPENCODE "\x1f"

vector *scan_line(void);
void prettify(FILE *file, FILE *out);

#endif