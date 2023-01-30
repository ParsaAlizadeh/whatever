#ifndef EDITOR_HIGHLIGHT_H
#define EDITOR_HIGHLIGHT_H

#include "../editor.h"

void editor_hl_reset(void);
void editor_hl_push(highlight_t *hl);
void editor_hl_adda(pos_t afrom, pos_t ato);
void editor_hl_addt(int tfrom, int n);
int editor_ishl(pos_t apos);
int editor_hl_next(void);

#endif
