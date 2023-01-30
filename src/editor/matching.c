#include "matching.h"

#include <stdlib.h>

static const char matches[][3] = { "()", "[]", "{}" };
static const int n_matches = sizeof(matches) / sizeof(matches[0]);

typedef struct _list {
    pos_t apos;
    char chr;
    struct _list *tail;
} list;

static list *cons(pos_t apos, char chr, list *tail) {
    list *this = malloc(sizeof(list));
    this->apos = apos;
    this->chr = chr;
    this->tail = tail;
    return this;
}

static void list_free(list *this) {
    if (this == NULL)
        return;
    list_free(this->tail);
    free(this);
}

static list *list_erase(list *this) {
    list *tail = this->tail;
    free(this);
    return tail;
}

static pos_t _vc_match(vecline *vc, pos_t apos) {
    pos_t now = { 0, 0 };
    pos_t result = { -1, 0 };
    list *stack = NULL;
    while (result.line == -1) {
        if (now.line >= vc_nlines(vc))
            break;
        line_t *line = vc_atline(vc, now.line);
        if (now.col >= line->size) {
            now.line++;
            now.col = 0;
            continue;
        }
        int chr = line->content[now.col];
        for (int i = 0; i < n_matches; i++) {
            if (chr == matches[i][0]) {
                stack = cons(now, chr, stack);
                break;
            }
            if (chr != matches[i][1])
                continue;
            if (!stack || stack->chr != matches[i][0])
                continue;
            if (pos_eq(now, apos))
                result = stack->apos;
            if (pos_eq(stack->apos, apos))
                result = now;
            stack = list_erase(stack);
        }
        now.col++;
    }
    list_free(stack);
    return result;
}

pos_t vc_match(vecline *vc, pos_t apos) {
    int chr = vc_at(vc, apos);
    int matchable = 0;
    for (int i = 0; !matchable && i < n_matches; i++)
        for (int j = 0; !matchable && j < 2; j++)
            matchable |= (chr == matches[i][j]);
    if (!matchable)
        return (pos_t) { -1, 0 };
    return _vc_match(vc, apos);
}

pos_t editor_curmatch(void) {
    return vc_match(ed->vc, ed->acur);
}
