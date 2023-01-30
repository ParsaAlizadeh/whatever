#include "offset.h"

int editor_minvisx(void) {
    return editor_dcurcol() - SCROLLXOFF + 1;
}

int editor_maxvisx(void) {
    return editor_dcurcol() + SCROLLXOFF - 1;
}

int editor_minvisy(void) {
    return editor_dcurline() - SCROLLYOFF + 1;
}

int editor_maxvisy(void) {
    return editor_dcurline() + SCROLLYOFF - 1;
}

#define _fixmin(off, min) do { \
        if ((min) < 0) (off) += (min); \
        if ((off) < 0) (off) = 0; \
    } while (0)
#define _fixmax(off, w, max) do { \
        if ((max) >= (w)) (off) += (max) - (w) + 1; \
    } while (0)
#define _fix(off, w, min, max) do { \
        _fixmin(off, min); \
        _fixmax(off, w, max); \
    } while (0)

void editor_fixoffset(void) {
    int h, w;
    getmaxyx(ed->fw, h, w);
    int minvisx = editor_minvisx();
    int maxvisx = editor_maxvisx();
    _fix(ed->off.col, w, minvisx, maxvisx);
    int minvisy = editor_minvisy();
    int maxvisy = editor_maxvisy();
    _fix(ed->off.line, h, minvisy, maxvisy);
}
