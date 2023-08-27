#ifndef PATTERN_H
#define PATTERN_H

#include <ctype.h>
#include "vector.h"

#define isstar(c) (!isspace(c))

typedef struct {
    vector *nodes;
    long visited;
} pattern;

extern pattern *pattern_new(const char *str);
extern void pattern_free(pattern *);
extern void pattern_feed(pattern *, char c, int start);
extern long pattern_matched(pattern *);
extern void pattern_reset(pattern *);
extern int pattern_search(pattern *, const char *str);
extern void pattern_log(pattern *);

#endif