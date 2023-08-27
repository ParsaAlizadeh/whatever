#ifndef EDITOR_HIGHLIGHT_H
#define EDITOR_HIGHLIGHT_H

#include "../editor.h"

extern void editor_hl_reset(void);
extern void editor_hl_push(highlight_t *hl);
extern void editor_hl_adda(pos_t afrom, pos_t ato);
extern void editor_hl_addt(int tfrom, int n);
extern int editor_ishl(pos_t apos);
extern int editor_hl_next(void);

#endif
