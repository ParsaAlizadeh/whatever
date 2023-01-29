#include "position.h"

#include <stdlib.h>

int pos_eq(pos_t a, pos_t b) {
    return a.line == b.line && a.col == b.col;
}

int pos_less(pos_t a, pos_t b) {
    if (a.line != b.line)
        return a.line < b.line;
    return a.col < b.col;
}

int pos_lesseq(pos_t a, pos_t b) {
    return pos_less(a, b) || pos_eq(a, b);
}

int pos_inrange(pos_t afrom, pos_t apos, pos_t ato) {
    if (afrom.line < 0 || ato.line < 0)
        return 0;
    if (pos_lesseq(afrom, apos) && pos_less(apos, ato))
        return 1;
    return pos_lesseq(ato, apos) && pos_less(apos, afrom);
}

highlight_t *hl_new(pos_t afrom, pos_t ato) {
    highlight_t *this = malloc(sizeof(highlight_t));
    this->afrom = afrom;
    this->ato = ato;
    return this;
}

int hl_inrange(highlight_t *this, pos_t apos) {
    return pos_inrange(this->afrom, apos, this->ato);
}
