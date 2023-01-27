#ifndef POSITION_H
#define POSITION_H

typedef struct {
    int line, col;
} pos_t;

int pos_lesseq(pos_t a, pos_t b);
int pos_inrange(pos_t afrom, pos_t apos, pos_t ato);

#endif
