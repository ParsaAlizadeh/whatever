#include "pattern.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct {
    char c;
    int isWild;
    long firstBefore;
    long firstAfter;
} node;

node *node_new(char c, int isWild) {
    node *this = malloc(sizeof(node));
    this->c = c;
    this->isWild = isWild;
    this->firstBefore = this->firstAfter = -1;
    return this;
}

static vector *build_nodes(const char *str, int size) {
    vector *nodes = vector_new();
    int escaped = 0, isWild = 0;
    for (int i = 0; i <= size; i++) {
        if (escaped) {
            vector_push(nodes, node_new(str[i], isWild));
            escaped = isWild = 0;
            continue;
        }
        if (str[i] == '\\') {
            escaped = 1;
            continue;
        }
        if (str[i] != '*')
            vector_push(nodes, node_new(str[i], isWild));
        isWild = (str[i] == '*');
    }
    return nodes;
}

pattern *pattern_new(const char *pat) {
    int size = strlen(pat);
    pattern *this = malloc(sizeof(pattern));
    this->nodes = build_nodes(pat, size);
    this->visited = 0;
    return this;
}

void pattern_free(pattern *this) {
    vector_freeall(this->nodes);
    free(this);
}

static void append(node *this, long first) {
    if (this->firstAfter == -1 || this->firstAfter > first)
        this->firstAfter = first;
}

static void push(node *this, node *next, char c) {
    if (this->firstBefore == -1)
        return;
    if (this->c == c && next != NULL)
        append(next, this->firstBefore);
    if (this->isWild && !isdelim(c))
        append(this, this->firstBefore);
}

static void step(node *this) {
    this->firstBefore = this->firstAfter;
    this->firstAfter = -1;
}

void pattern_feed(pattern *this, char c, int start) {
    node *first = this->nodes->seq[0];
    if (start && first->firstBefore == -1) {
        first->firstBefore = this->visited;
    }
    for (int i = 0; i < this->nodes->size; i++) {
        node *cur = this->nodes->seq[i];
        node *nxt = this->nodes->seq[i+1];
        push(cur, nxt, c);
    }
    for (int i = 0; i < this->nodes->size; i++) {
        node *cur = this->nodes->seq[i];
        step(cur);
    }
    this->visited++;
}

long pattern_matched(pattern *this) {
    int size = this->nodes->size;
    node *last = this->nodes->seq[size-1];
    return last->firstBefore;
}

void pattern_reset(pattern *this) {
    this->visited = 0;
    for (int i = 0; i < this->nodes->size; i++) {
        node *cur = this->nodes->seq[i];
        cur->firstBefore = cur->firstAfter = -1;
    }
}

int pattern_search(pattern *this, const char *str) {
    while (*str) {
        pattern_feed(this, *(str++), 1);
        if (pattern_matched(this) != -1) {
            pattern_reset(this);
            return 1;
        }
    }
    pattern_reset(this);
    return 0;
}

void pattern_log(pattern *this) {
    for (int i = 0; i < this->nodes->size; i++) {
        node *n = this->nodes->seq[i];
        fprintf(stderr, "c=%c, isWild=%d, firstB=%ld, firstA=%ld\n",
            n->c, n->isWild, n->firstBefore, n->firstAfter);
    }
    fprintf(stderr, "===\n");
}
