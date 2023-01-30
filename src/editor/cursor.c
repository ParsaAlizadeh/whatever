#include "cursor.h"

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

void editor_set_tcur(int tcur) {
    ed->acur = vc_fromtpos(ed->vc, tcur);
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

void editor_insert(char chr) {
    editor_hl_reset();
    ed->acur = vc_insert(ed->vc, ed->acur, chr);
    ed->modified = 1;
}

void editor_erase(void) {
    editor_hl_reset();
    ed->acur = vc_erase(ed->vc, ed->acur);
    ed->modified = 1;
}
