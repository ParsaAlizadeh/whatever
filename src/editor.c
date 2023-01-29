#include "editor.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <err.h>
#include "context.h"
#include "fileutil.h"
#include "chain.h"
#include "parse.h"
#include "vecstr.h"

EDITOR *ed = NULL;

void editor_new(WINDOW *frame) {
    int h, w;
    getmaxyx(frame, h, w);
    if (h < 4)
        return;
    if (w < LINE_WIDTH + 2)
        return;

    ed = malloc(sizeof(EDITOR));

    ed->frame = frame;
    wrefresh(frame);
    ed->lw = derwin(frame, h-3, LINE_WIDTH, 0, 0);
    ed->fw = derwin(frame, h-3, w-LINE_WIDTH-1, 0, LINE_WIDTH+1);
    ed->iw = derwin(frame, 1, w, h-2, 0);
    ed->cw = derwin(frame, 1, w, h-1, 0);

    ed->off = (pos_t) { 0, 0 };
    ed->acur = (pos_t) { 0, 0 };
    ed->vc = vc_new1();
    editor_reset();
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

void editor_reset() {
    ed->asel = (pos_t) { -1, 0 };
    ed->modified = 0;
    editor_mvcur();
}

void editor_setvc(vecline *vc) {
    vc_free(ed->vc);
    ed->vc = vc;
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

int editor_ishl(pos_t apos) {
    if (pos_inrange(ed->asel, apos, ed->acur))
        return 1;
    /* some other checks */
    return 0;
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
    int ishl = editor_ishl(apos);
    wmove(ed->fw, dpos.line, dpos.col);
    if (ishl)
        wattron(ed->fw, COLOR_PAIR(2));
    if (chr == '\t') {
        for (int i = 0; i < TAB_STOP; i++)
            waddch(ed->fw, ' ');
    } else if (chr == '\0' || isspace(chr)) {
        waddch(ed->fw, ' ');
    } else {
        waddch(ed->fw, chr);
    }
    if (ishl)
        wattroff(ed->fw, COLOR_PAIR(2));
}

void editor_printline(int aline) {
    int h = getmaxy(ed->lw);
    if (aline < ed->off.line || aline >= ed->off.line+h)
        return;
    mvwprintw(ed->lw, aline - ed->off.line, 0, LINE_FORMAT, aline+1);
    pos_t apos = { aline, 0 };
    int size = vc_atline(ed->vc, aline)->size;
    for (; apos.col <= size; apos.col++)
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
    editor_adjustcur();
    pos_t dcur = editor_dcur();
    if (!wvisible(ed->fw, dcur))
        return;
    wmove(ed->fw, dcur.line, dcur.col);
    wrefresh(ed->fw);
}

void editor_adjustcur(void) {
    int nlines = vc_nlines(ed->vc);
    if (ed->acur.line < 0)
        ed->acur.line = 0;
    if (ed->acur.line >= nlines)
        ed->acur.line = nlines-1;
    int ncols = vc_atline(ed->vc, ed->acur.line)->size;
    if (ed->acur.col < 0)
        ed->acur.col = 0;
    if (ed->acur.col > ncols)
        ed->acur.col = ncols;
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

void editor_printinfo(void) {
    wclear(ed->iw);
    wmove(ed->iw, 0, 1);
    const char *edmode = ctx_get_edmode();
    if (edmode != NULL)
        wprintw(ed->iw, "%s", edmode);
    else
        wprintw(ed->iw, "?????");
    waddch(ed->iw, ' ');
    if (ctx_get_buf_mode())
        wprintw(ed->iw, "(buffer)");
    else {
        wprintw(ed->iw, "%s", ctx_get());
        if (ed->modified)
            wprintw(ed->iw, " +");
    }
}

void editor_refresh(void) {
    editor_fixoffset();
    editor_printfile();
    editor_printinfo();
    editor_printborder();
    wrefresh(ed->fw);
    wrefresh(ed->lw);
    wrefresh(ed->iw);
    wrefresh(ed->cw);
    wrefresh(ed->frame);
    editor_mvcur();
}

void editor_insert(char chr) {
    ed->acur = vc_insert(ed->vc, ed->acur, chr);
    ed->modified = 1;
}

void editor_erase(void) {
    ed->acur = vc_erase(ed->vc, ed->acur);
    ed->modified = 1;
}

int editor_saveas(const char *path) {
    return vc_writepath(ed->vc, path);
}

int editor_loadctx(void) {
    vecline *vc = vc_newpath(ctx_get());
    if (vc == NULL)
        return -1;
    editor_setvc(vc);
    return 0;
}

void editor_clearbuffer(void) {
    unlink(BUFFER_PATH);
}

void editor_run_init(void) {
    ctx_set_buf_mode(1);
    ctx_save();
}

void editor_run_end(char *out) {
    if (ctx_get_buf_mode()) {
        ed->modified = 1;
        editor_loadctx();
    }
    ctx_set_buf_mode(0);
    if (out != NULL) {
        ctx_set(NULL);
        editor_setvc(vc_newstr(out));
        editor_reset();
        free(out);
    }
}

void editor_run(vector *tokens) {
    editor_run_init();
    char *out = NULL;
    procedure_chain(tokens, &out);
    editor_run_end(out);
}

void editor_runf(const char *format, ...) {
    string *cmd = string_new();
    va_list args;
    va_start(args, format);
    vfprintf(cmd->f, format, args);
    va_end(args);
    char *strcmd = string_free(cmd);
    vector *tokens = scan_strline(strcmd);
    editor_run(tokens);
    vector_freeall(tokens);
    free(strcmd);
}

void init_ncurses(void) {
    initscr();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);

    editor_new(stdscr);
    if (ed == NULL)
        errx(EXIT_FAILURE, "editor_new");

    editor_clearbuffer();
    editor_loadctx();
}

void end_ncurses(void) {
    editor_free();
    endwin();
}
