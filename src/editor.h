#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include "vecline.h"

#define TAB_STOP        8
#define SCROLLYOFF      4
#define SCROLLXOFF      TAB_STOP
#define LINE_WIDTH      5
#define LINE_FORMAT     "%d"

enum {
    COLOR_TXT = 1,
    COLOR_HL,
    COLOR_PAREN,
    COLOR_BRACKET,
    COLOR_CURLY,
    COLOR_MATCH
};

#define COLOR_TXT_P     COLOR_WHITE, COLOR_BLACK
#define COLOR_HL_P      COLOR_WHITE, COLOR_YELLOW
#define COLOR_PAREN_P   COLOR_BLUE, COLOR_BLACK
#define COLOR_BRACKET_P COLOR_GREEN, COLOR_BLACK
#define COLOR_CURLY_P   COLOR_MAGENTA, COLOR_BLACK
#define COLOR_MATCH_P   COLOR_BLACK, COLOR_WHITE

typedef struct {
    WINDOW *frame;
    WINDOW *lw, *fw, *iw, *cw; /* line, file, info, command */
    vecline *vc;
    pos_t off, acur;
    int modified;
    pos_t asel;
    vector *hls;
} EDITOR;

extern EDITOR *ed;

#include "editor/object.h"
#include "editor/position.h"
#include "editor/highlight.h"
#include "editor/print.h"
#include "editor/cursor.h"
#include "editor/offset.h"
#include "editor/movement.h"
#include "editor/buffer.h"
#include "editor/run.h"
#include "editor/matching.h"

#endif
