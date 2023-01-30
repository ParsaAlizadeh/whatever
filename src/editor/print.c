#include "print.h"

#include <ctype.h>
#include "../context.h"

int wvisible(WINDOW *win, pos_t dpos) {
    int h, w;
    getmaxyx(win, h, w);
    if (dpos.line < 0 || dpos.line >= h)
        return 0;
    if (dpos.col < 0 || dpos.col >= w)
        return 0;
    return 1;
}

static int colorof(pos_t apos, char chr) {
    if (editor_ishl(apos))
        return COLOR_HL;
    if (pos_eq(apos, editor_curmatch()))
        return COLOR_MATCH;
    if (chr == '(' || chr == ')')
        return COLOR_PAREN;
    if (chr == '[' || chr == ']')
        return COLOR_BRACKET;
    if (chr == '{' || chr == '}')
        return COLOR_CURLY;
    return COLOR_TXT;
}

void editor_printc(pos_t apos) {
    pos_t dpos = editor_dpos(apos);
    if (!wvisible(ed->fw, dpos))
        return;
    char chr = vc_at(ed->vc, apos);
    int color = colorof(apos, chr);
    wmove(ed->fw, dpos.line, dpos.col);
    wattron(ed->fw, COLOR_PAIR(color));
    if (chr == '\t') {
        for (int i = 0; i < TAB_STOP; i++)
            waddch(ed->fw, ' ');
    } else if (chr == '\0' || isspace(chr)) {
        waddch(ed->fw, ' ');
    } else {
        waddch(ed->fw, chr);
    }
    wattroff(ed->fw, COLOR_PAIR(color));
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

void editor_printborder(void) {
    mvwvline(ed->frame, 0, getmaxx(ed->lw), ACS_VLINE, getmaxy(ed->lw));
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
    mvwchgat(ed->iw, 0, 0, getmaxx(ed->iw), A_REVERSE, COLOR_TXT, NULL);
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
