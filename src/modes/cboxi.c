#include "cboxi.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "movement.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

static char indicator;
static line_t *line;
static int cursor;

void cboxi_init(char c) {
    indicator = c;
    line = line_new(strdup(""), 0);
    cursor = 0;
}

void cboxi_end(void) {
    line_free(line);
}

static void cboxi_left(void) {
    if (cursor > 0)
        cursor--;
}

static void cboxi_right(void) {
    if (cursor < line->size)
        cursor++;
}

static void cboxi_insert(char chr) {
    line_insert(line, cursor, chr);
    cursor++;
}

static void cboxi_erase(void) {
    if (cursor == 0)
        return;
    line_erase(line, cursor-1);
    cursor--;
}

static void cboxi_writeline(void) {
    wclear(ed->cw);
    wmove(ed->cw, 0, 0);
    waddch(ed->cw, indicator);
    for (int i = 0; i < line->size; i++) {
        if (isspace(line->content[i]))
            waddch(ed->cw, ' ');
        else
            waddch(ed->cw, line->content[i]);
    }
}

static void cboxi_mvcur(void) {
    wmove(ed->cw, 0, cursor+1);
}

cboxi_rc_t cboxi_refresh(void) {
    cboxi_writeline();
    cboxi_mvcur();
    editor_refresh();
    wrefresh(ed->cw);
    int chr = getch();
    switch (chr) {
    case KEY_F(1):
        return CBOXI_CANCEL;
    case '\n':
        return CBOXI_DONE;
    case KEY_LEFT:
        cboxi_left();
        break;
    case KEY_RIGHT:
        cboxi_right();
        break;
    case KEY_BACKSPACE:
        cboxi_erase();
        break;
    default:
        if (isgraph(chr) || isspace(chr))
            cboxi_insert(chr);
    }
    return CBOXI_CONTINUE;
}

const char *cboxi_content(void) {
    return line->content;
}

FILE *cboxi_fcontent(void) {
    return fmemopen(line->content, line->size, "r");
}
