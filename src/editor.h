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
    int modified;
    pos_t asel;
} EDITOR;

extern EDITOR *ed;

void editor_new(WINDOW *frame);
void editor_free(void);
void editor_reset(void);
void editor_setvc(vecline *vc);

int editor_dline(pos_t apos);
int editor_dcol(pos_t apos);
pos_t editor_dpos(pos_t apos);
int editor_acol(int aline, int dcol);

int editor_ishl(pos_t apos);

void editor_printc(pos_t apos);
void editor_printline(int aline);
void editor_printfile(void);

line_t *editor_curline(void);
int editor_dcurline(void);
int editor_dcurcol(void);
pos_t editor_dcur(void);
void editor_mvcur(void);
void editor_adjustcur(void);

int editor_minvisx(void);
int editor_maxvisx(void);
int editor_minvisy(void);
int editor_maxvisy(void);
void editor_fixoffset(void);

void editor_left(void);
void editor_right(void);
void editor_up(void);
void editor_down(void);
void editor_home(void);
void editor_end(void);

void editor_printborder(void);
void editor_printinfo(void);
void editor_refresh(void);

void editor_insert(char chr);
void editor_erase(void);

int editor_saveas(const char *path);
int editor_loadctx(void);
void editor_clearbuffer(void);

void editor_run_init(void);
void editor_run_end(char *out);
void editor_run(vector *tokens);
void editor_runf(const char *format, ...);

void init_ncurses(void);
void end_ncurses(void);

#endif
