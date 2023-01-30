#include "object.h"

#include <stdlib.h>
#include <err.h>

EDITOR *ed = NULL;

void editor_new(WINDOW *frame) {
    int h, w;
    getmaxyx(frame, h, w);
    if (h < 3)
        return;
    if (w < LINE_WIDTH + 2)
        return;

    ed = malloc(sizeof(EDITOR));

    ed->frame = frame;
    wrefresh(frame);
    ed->lw = derwin(frame, h-2, LINE_WIDTH, 0, 0);
    ed->fw = derwin(frame, h-2, w-LINE_WIDTH-1, 0, LINE_WIDTH+1);
    ed->iw = derwin(frame, 1, w, h-2, 0);
    ed->cw = derwin(frame, 1, w, h-1, 0);

    ed->off = (pos_t) { 0, 0 };
    ed->acur = (pos_t) { 0, 0 };
    ed->vc = vc_new1();

    ed->hls = NULL;

    editor_reset();
}

void editor_free(void) {
    editor_hl_reset();
    delwin(ed->lw);
    delwin(ed->fw);
    delwin(ed->iw);
    delwin(ed->cw);
    vc_free(ed->vc);
    free(ed);
    ed = NULL;
}

void init_ncurses(void) {
    initscr();
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(COLOR_TXT, COLOR_TXT_P);
    init_pair(COLOR_HL, COLOR_HL_P);
    init_pair(COLOR_PAREN, COLOR_PAREN_P);
    init_pair(COLOR_BRACKET, COLOR_BRACKET_P);
    init_pair(COLOR_CURLY, COLOR_CURLY_P);

    editor_new(stdscr);
    if (ed == NULL)
        errx(EXIT_FAILURE, "editor_new failed");

    editor_unlinkbuffer();
    editor_loadctx();
}

void end_ncurses(void) {
    editor_free();
    endwin();
}
