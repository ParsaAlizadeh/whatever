#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include "vecline.h"

#define TAB_STOP        4
#define SCROLLYOFF      3
#define SCROLLXOFF      TAB_STOP
#define LINE_WIDTH      5
#define LINE_FORMAT     "%-5d"

#define COLOR_TXT       1
#define COLOR_TXT_P     COLOR_WHITE, COLOR_BLACK
#define COLOR_HL        2
#define COLOR_HL_P      COLOR_WHITE, COLOR_YELLOW
#define COLOR_PAREN     3
#define COLOR_PAREN_P   COLOR_BLUE, COLOR_BLACK
#define COLOR_BRACKET   4
#define COLOR_BRACKET_P COLOR_GREEN, COLOR_BLACK
#define COLOR_CURLY     5
#define COLOR_CURLY_P   COLOR_MAGENTA, COLOR_BLACK

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

#endif
