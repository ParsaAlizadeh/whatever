#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include "vecline.h"

#define TAB_STOP        4
#define SCROLLYOFF      3
#define SCROLLXOFF      TAB_STOP
#define LINE_WIDTH      4
#define LINE_FORMAT     "%-4d"

typedef struct {
    WINDOW *frame;
    WINDOW *lw, *fw, *iw, *cw; /* line, file, info, command */
    vecline *vc;
    pos_t off, acur;
} EDITOR;

EDITOR *editor_new(WINDOW *frame);
void editor_free(EDITOR *ed);
void editor_setvc(EDITOR *ed, vecline *vc);

int editor_dline(EDITOR *ed, pos_t apos);
int editor_dcol(EDITOR *ed, pos_t apos);
pos_t editor_dpos(EDITOR *ed, pos_t apos);
int editor_acol(EDITOR *ed, int aline, int dcol);

void editor_printc(EDITOR *ed, pos_t apos);
void editor_printline(EDITOR *ed, int aline);
void editor_printfile(EDITOR *ed);

line_t *editor_curline(EDITOR *ed);
int editor_dcurline(EDITOR *ed);
int editor_dcurcol(EDITOR *ed);
pos_t editor_dcur(EDITOR *ed);
void editor_mvcur(EDITOR *ed);

int editor_minvisx(EDITOR *ed);
int editor_maxvisx(EDITOR *ed);
int editor_minvisy(EDITOR *ed);
int editor_maxvisy(EDITOR *ed);
void editor_fixoffset(EDITOR *ed);

void editor_left(EDITOR *ed);
void editor_right(EDITOR *ed);
void editor_up(EDITOR *ed);
void editor_down(EDITOR *ed);
void editor_home(EDITOR *ed);
void editor_end(EDITOR *ed);

void editor_printborder(EDITOR *ed);
void editor_refresh(EDITOR *ed);

void editor_insert(EDITOR *ed, char chr);
void editor_erase(EDITOR *ed);

void init_ncurses(void);
void end_ncurses(void);

#endif
