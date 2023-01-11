#ifndef PATTERN_H
#define PATTERN_H

#include "vector.h"

typedef struct {
    vector *nodes;
    long visited;
    int wildsuffix;
} pattern;

pattern *pattern_new(const char *str);
void pattern_free(pattern *this);
void pattern_feed(pattern *this, int c);
long pattern_matched(pattern *this);
void pattern_reset(pattern *this);
int pattern_search(pattern *this, const char *str);
void pattern_log(pattern *this);

#endif