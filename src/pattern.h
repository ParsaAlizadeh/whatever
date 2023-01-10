#ifndef PATTERN_H
#define PATTERN_h

typedef struct {
    char *str;
    int *kmp;
    int size;
    int visited, current;
    int wildprefix, wildsuffix;
} pattern;

pattern *pattern_new(const char *str);
void pattern_free(pattern *this);
void pattern_feed(pattern *this, int c);
int pattern_matched(pattern *this);

#endif