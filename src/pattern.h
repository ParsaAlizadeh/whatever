#ifndef PATTERN_H
#define PATTERN_h

typedef struct {
    char *str;
    int *kmp;
    long visited;
    int size;
    int current;
    int wildprefix, wildsuffix;
} pattern;

pattern *pattern_new(const char *str);
void pattern_free(pattern *this);
void pattern_feed(pattern *this, int c);
int pattern_matched(pattern *this);
long pattern_start(pattern *this);

#endif