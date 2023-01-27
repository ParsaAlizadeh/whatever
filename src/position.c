#include "position.h"

#include <stdlib.h>

int pos_lesseq(pos_t a, pos_t b) {
    if (a.line != b.line)
        return a.line < b.line;
    return a.col <= b.col;
}

int pos_inrange(pos_t afrom, pos_t apos, pos_t ato) {
    if (afrom.line < 0 || ato.line < 0)
        return 0;
    if (pos_lesseq(afrom, apos) && pos_lesseq(apos, ato))
        return 1;
    return pos_lesseq(ato, apos) && pos_lesseq(apos, afrom);
}
