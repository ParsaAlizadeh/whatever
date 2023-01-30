#include "highlight.h"

void editor_hl_reset(void) {
    if (ed->hls != NULL)
        vector_freeall(ed->hls);
    ed->hls = NULL;
}

void editor_hl_push(highlight_t *hl) {
    if (ed->hls == NULL)
        ed->hls = vector_new();
    vector_push(ed->hls, hl);
}

void editor_hl_adda(pos_t afrom, pos_t ato) {
    editor_hl_push(hl_new(afrom, ato));
}

void editor_hl_addt(int tfrom, int n) {
    pos_t afrom = vc_fromtpos(ed->vc, tfrom);
    pos_t ato = vc_fromtpos(ed->vc, tfrom + n);
    editor_hl_adda(afrom, ato);
}

int editor_ishl(pos_t apos) {
    if (pos_inrange(ed->asel, apos, ed->acur))
        return 1;
    if (ed->hls == NULL)
        return 0;
    for (int i = 0; i < ed->hls->size; i++) {
        highlight_t *hl = ed->hls->seq[i];
        if (hl_inrange(hl, apos))
            return 1;
    }
    return 0;
}

int editor_hl_next(void) {
    if (ed->hls == NULL || ed->hls->size == 0)
        return -1;
    for (int i = 0; i < ed->hls->size; i++) {
        highlight_t *hl = ed->hls->seq[i];
        if (pos_less(ed->acur, hl->afrom)) {
            ed->acur = hl->afrom;
            return 0;
        }
    }
    highlight_t *first = ed->hls->seq[0];
    ed->acur = first->afrom;
    return 0;
}
