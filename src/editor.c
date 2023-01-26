#include "editor.h"

#include <stdlib.h>
#include <ctype.h>
#include "context.h"

EDITOR *ed = NULL;

EDITOR *editor_new(WINDOW *frame) {
    int h, w;
    getmaxyx(frame, h, w);
    if (h < 4)
        return NULL;
    if (w < LINE_WIDTH + 2)
        return NULL;

    EDITOR *ed = malloc(sizeof(EDITOR));
    ed->off = (pos_t) { 0, 0 };
    ed->acur = (pos_t) { 0, 0 };

    ed->frame = frame;
    wrefresh(frame);
    ed->lw = derwin(frame, h-3, LINE_WIDTH, 0, 0);
    ed->fw = derwin(frame, h-3, w-LINE_WIDTH-1, 0, LINE_WIDTH+1);
    ed->iw = derwin(frame, 1, w, h-2, 0);
    ed->cw = derwin(frame, 1, w, h-1, 0);

    ed->vc = vc_new1();

    return ed;
}

void editor_free(void) {
    delwin(ed->lw);
    delwin(ed->fw);
    delwin(ed->iw);
    delwin(ed->cw);
    vc_free(ed->vc);
    free(ed);
    ed = NULL;
}

void editor_setvc(vecline *vc) {
    vc_free(ed->vc);
    ed->vc = vc;
    ed->acur = (pos_t) { 0, 0 };
    ed->off = (pos_t) { 0, 0 };
}

int editor_dline(pos_t apos) {
    return apos.line - ed->off.line;
}

int editor_dcol(pos_t apos) {
    line_t *line = vc_atline(ed->vc, apos.line);
    if (line == NULL)
        return -1;
    if (apos.col > line->size)
        return -1;
    int dcol = 0;
    for (int i = 0; i < apos.col; i++) {
        switch (line->content[i]) {
        case '\t':
            dcol += TAB_STOP - (dcol % TAB_STOP);
            break;
        default:
            dcol++;
            break;
        }
    }
    return dcol - ed->off.col;
}

pos_t editor_dpos(pos_t apos) {
    return (pos_t) {
        .line = editor_dline(apos),
        .col  = editor_dcol(apos)
    };
}

int editor_acol(int aline, int dcol) {
    line_t *line = vc_atline(ed->vc, aline);
    if (line == NULL)
        return -1;
    int dcurcol = -ed->off.col, nxt;
    for (int i = 0; i < line->size; i++) {
        switch (line->content[i]) {
        case '\t':
            nxt = TAB_STOP - (dcurcol % TAB_STOP);
            break;
        default:
            nxt = 1;
            break;
        }
        if (dcurcol + nxt > dcol)
            return i;
        dcurcol += nxt;
    }
    return line->size;
}

static int wvisible(WINDOW *win, pos_t dpos) {
    int h, w;
    getmaxyx(win, h, w);
    if (dpos.line < 0 || dpos.line >= h)
        return 0;
    if (dpos.col < 0 || dpos.col >= w)
        return 0;
    return 1;
}

void editor_printc(pos_t apos) {
    pos_t dpos = editor_dpos(apos);
    if (!wvisible(ed->fw, dpos))
        return;
    char chr = vc_at(ed->vc, apos);
    if (isspace(chr))
        return;
    mvwaddch(ed->fw, dpos.line, dpos.col, chr);
}

void editor_printline(int aline) {
    int h = getmaxy(ed->lw);
    if (aline < ed->off.line || aline >= ed->off.line+h)
        return;
    mvwprintw(ed->lw, aline - ed->off.line, 0, LINE_FORMAT, aline+1);
    pos_t apos = { aline, 0 };
    int size = vc_atline(ed->vc, aline)->size;
    for (; apos.col < size; apos.col++)
        editor_printc(apos);
}

void editor_printfile(void) {
    wclear(ed->fw);
    wclear(ed->lw);
    for (int aline = 0; aline < vc_nlines(ed->vc); aline++)
        editor_printline(aline);
}

line_t *editor_curline(void) {
    return vc_atline(ed->vc, ed->acur.line);
}

int editor_dcurline(void) {
    return editor_dline(ed->acur);
}

int editor_dcurcol(void) {
    return editor_dcol(ed->acur);
}

pos_t editor_dcur(void) {
    return (pos_t) {
        .line = editor_dcurline(),
        .col  = editor_dcurcol()
    };
}

void editor_mvcur(void) {
    pos_t dcur = editor_dcur();
    if (!wvisible(ed->fw, dcur))
        return;
    wmove(ed->frame, dcur.line, LINE_WIDTH + dcur.col + 1);
}

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

void editor_left(void) {
    if (ed->acur.col == 0)
        return;
    ed->acur.col--;
}

void editor_right(void) {
    line_t *line = editor_curline();
    if (ed->acur.col == line->size)
        return;
    ed->acur.col++;
}

void editor_up(void) {
    if (ed->acur.line == 0)
        return;
    int prvdcurcol = editor_dcurcol();
    ed->acur.line--;
    ed->acur.col = editor_acol(ed->acur.line, prvdcurcol);
}

void editor_down(void) {
    if (ed->acur.line == vc_nlines(ed->vc)-1)
        return;
    int prvdcurcol = editor_dcurcol();
    ed->acur.line++;
    ed->acur.col = editor_acol(ed->acur.line, prvdcurcol);
}

void editor_home(void) {
    ed->acur.col = 0;
}

void editor_end(void) {
    ed->acur.col = editor_curline()->size;
}

void editor_printborder(void) {
    mvwvline(ed->frame, 0, getmaxx(ed->lw), ACS_VLINE, getmaxy(ed->lw));
    mvwhline(ed->frame, getmaxy(ed->lw), 0, ACS_HLINE, getmaxx(ed->lw));
    mvwhline(ed->frame, getmaxy(ed->lw), getmaxx(ed->lw)+1, ACS_HLINE, getmaxx(ed->fw));
    mvwaddch(ed->frame, getmaxy(ed->lw), getmaxx(ed->lw), ACS_BTEE);
}

void editor_refresh(void) {
    editor_fixoffset();
    editor_printfile();
    editor_printborder();
    editor_mvcur();
    wrefresh(ed->fw);
    wrefresh(ed->lw);
    wrefresh(ed->iw);
    wrefresh(ed->frame);
}

void editor_insert(char chr) {
    ed->acur = vc_insert(ed->vc, ed->acur, chr);
}

void editor_erase(void) {
    ed->acur = vc_erase(ed->vc, ed->acur);
}

void init_ncurses(void) {
    initscr();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    ed = editor_new(stdscr);
    const char *ctx = ctx_get();
    if (ctx != NULL) {
        vecline *vc = vc_newpath(ctx);
        if (vc != NULL)
            editor_setvc(vc);
    }
}

void end_ncurses(void) {
    editor_free();
    endwin();
}
