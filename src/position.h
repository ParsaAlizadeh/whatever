#ifndef POSITION_H
#define POSITION_H

typedef struct {
    int line, col;
} pos_t;

typedef struct {
    pos_t afrom, ato;
} highlight_t;

extern int pos_eq(pos_t, pos_t);
extern int pos_less(pos_t, pos_t);
extern int pos_lesseq(pos_t, pos_t);

/*
 * checks for afrom <= apos < ato or ato <= apos < aform
 */
extern int pos_inrange(pos_t afrom, pos_t apos, pos_t ato);

extern highlight_t *hl_new(pos_t afrom, pos_t ato);
extern int hl_inrange(highlight_t *, pos_t apos);

#endif
