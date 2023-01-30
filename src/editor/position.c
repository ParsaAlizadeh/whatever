#include "position.h"

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
