#ifndef EDITOR_CURSOR_H
#define EDITOR_CURSOR_H

#include "../editor.h"

line_t *editor_curline(void);
int editor_dcurline(void);
int editor_dcurcol(void);
pos_t editor_dcur(void);
void editor_set_tcur(int tcur);
void editor_mvcur(void);
void editor_adjustcur(void);

void editor_insert(char chr);
void editor_erase(void);

#endif
