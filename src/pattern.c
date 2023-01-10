#include "pattern.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vecstr.h"

static char *escape_wildcard(const char *str, int size) {
    string *res = string_new();
    int escaped = 0;
    for (int i = 0; i < size; i++) {
        if (escaped) {
            string_push(res, str[i]);
            escaped = 0;
            continue;
        }
        if (str[i] == '\\') {
            escaped = 1;
            continue;
        }
        if (str[i] != '*')
            string_push(res, str[i]);
    }
    return string_free(res);
}

static int *build_kmp(char *str, int n) {
    int *kmp = malloc(sizeof(int) * n);
    kmp[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = kmp[i-1];
        while (j > 0 && str[i] != str[j])
            j = kmp[j-1];
        if (str[i] == str[j])
            j++;
        kmp[i] = j;
    }
    return kmp;
}

pattern *pattern_new(const char *pat) {
    int size = strlen(pat);
    if (size == 0 || (size == 1 && pat[0] == '*'))
        return NULL;
    pattern *this = malloc(sizeof(pattern));
    this->str = escape_wildcard(pat, size);
    this->size = strlen(this->str);
    this->visited = this->current = 0;
    this->wildprefix = (pat[0] == '*');
    this->wildsuffix = (pat[size-1] == '*') && (pat[size-2] != '\\');
    this->kmp = build_kmp(this->str, this->size);
    return this;
}

void pattern_free(pattern *this) {
    free(this->str);
    free(this->kmp);
    free(this);
}

void pattern_feed(pattern *this, int c) {
    int *cur = &this->current;
    while (*cur > 0 && this->str[*cur] != c)
        *cur = this->kmp[(*cur)-1];
    if (this->str[*cur] == c)
        (*cur)++;
    this->visited++;
}

int pattern_matched(pattern *this) {
    return this->current == this->size;
}

long pattern_start(pattern *this) {
    return this->visited - this->size;
}

void pattern_reset(pattern *this) {
    this->current = 0;
    this->visited = 0;
}

int pattern_search(pattern *this, const char *str) {
    while (*str) {
        pattern_feed(this, *(str++));
        if (pattern_matched(this)) {
            pattern_reset(this);
            return 1;
        }
    }
    pattern_reset(this);
    return 0;
}
