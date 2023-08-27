#ifndef EDITOR_CURSOR_H
#define EDITOR_CURSOR_H

#include "../editor.h"

extern line_t *editor_curline(void);
extern int editor_dcurline(void);
extern int editor_dcurcol(void);
extern pos_t editor_dcur(void);
extern void editor_set_tcur(int tcur);
extern void editor_mvcur(void);
extern void editor_adjustcur(void);

extern void editor_insert(char chr);
extern void editor_erase(void);

#endif
