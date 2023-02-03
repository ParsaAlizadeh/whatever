#include "matching.h"

#include <stdlib.h>

static const char matches[][3] = { "()", "[]", "{}" };
static const int n_matches = sizeof(matches) / sizeof(matches[0]);

#define N 200

typedef struct {
    char chr;
    pos_t apos;
} item;

static item stack[N];
static int ind;

static void init(void) {
    ind = 0;
}

static void push(item i) {
    if (ind >= N)
        return;
    stack[ind++] = i;
}

static item top(void) {
    return stack[ind-1];
}

static item pop(void) {
    return stack[--ind];
}

static int movedir(vecline *vc, pos_t *apos, int dir) {
    if (dir == 0) {
        line_t *line = vc_atline(vc, apos->line);
        apos->col++;
        if (apos->col == line->size) {
            apos->col = 0;
            apos->line++;
        }
        if (apos->line >= vc_nlines(vc))
            return -1;
        return 0;
    }
    apos->col--;
    if (apos->col < 0) {
        apos->line--;
        if (apos->line < 0)
            return -1;
        apos->col = vc_atline(vc, apos->line)->size;
    }
    return 0;
}

static pos_t _vc_match(vecline *vc, pos_t apos, int dir) {
    pos_t now = apos;
    pos_t result = { -1, 0 };
    init();
    while (result.line == -1) {
        char chr = vc_at(vc, now);
        for (int i = 0; i < n_matches; i++) {
            if (chr == matches[i][dir]) {
                push((item) { chr, now });
                break;
            }
            if (chr != matches[i][1^dir])
                continue;
            item tp = top();
            if (tp.chr != matches[i][dir])
                continue;
            if (pos_eq(tp.apos, apos))
                result = now;
            pop();
        }
        if (movedir(vc, &now, dir) == -1)
            break;
    }
    return result;
}

pos_t vc_match(vecline *vc, pos_t apos) {
    char chr = vc_at(vc, apos);
    for (int i = 0; i < n_matches; i++)
        for (int j = 0; j < 2; j++)
            if (chr == matches[i][j])
                return _vc_match(vc, apos, j);
    return (pos_t) { -1, 0 };
}

pos_t editor_curmatch(void) {
    return vc_match(ed->vc, ed->acur);
}
