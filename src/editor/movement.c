#include "movement.h"

void editor_toleft(void) {
    if (ed->acur.col == 0)
        return;
    ed->acur.col--;
}

void editor_toright(void) {
    line_t *line = editor_curline();
    if (ed->acur.col == line->size)
        return;
    ed->acur.col++;
}

void editor_toup(void) {
    if (ed->acur.line == 0)
        return;
    int prvdcurcol = editor_dcurcol();
    ed->acur.line--;
    ed->acur.col = editor_acol(ed->acur.line, prvdcurcol);
}

void editor_todown(void) {
    if (ed->acur.line == vc_nlines(ed->vc)-1)
        return;
    int prvdcurcol = editor_dcurcol();
    ed->acur.line++;
    ed->acur.col = editor_acol(ed->acur.line, prvdcurcol);
}

void editor_tohome(void) {
    ed->acur.col = 0;
}

void editor_toend(void) {
    ed->acur.col = editor_curline()->size;
}

void editor_tostart(void) {
    ed->acur = (pos_t) { 0, 0 };
}

void editor_tofinish(void) {
    int n_lines = vc_nlines(ed->vc);
    line_t *line = vc_atline(ed->vc, n_lines-1);
    ed->acur = (pos_t) { n_lines-1, line->size };
}

void editor_toprvmatch(void) {
    pos_t match = editor_curmatch();
    if (match.line == -1 || pos_less(ed->acur, match))
        return;
    ed->acur = match;
}

void editor_tonxtmatch(void) {
    pos_t match = editor_curmatch();
    if (match.line == -1 || pos_less(match, ed->acur))
        return;
    ed->acur = match;
}
