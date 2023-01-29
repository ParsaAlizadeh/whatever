#ifndef POSITION_H
#define POSITION_H

typedef struct {
    int line, col;
} pos_t;

typedef struct {
    pos_t afrom, ato;
} highlight_t;

int pos_eq(pos_t, pos_t);
int pos_less(pos_t, pos_t);
int pos_lesseq(pos_t, pos_t);

/*
 * checks for afrom <= apos < ato or ato <= apos < aform
 */
int pos_inrange(pos_t afrom, pos_t apos, pos_t ato);

highlight_t *hl_new(pos_t afrom, pos_t ato);
int hl_inrange(highlight_t *this, pos_t apos);

#endif
