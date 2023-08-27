#ifndef EDITOR_PRINT_H
#define EDITOR_PRINT_H

#include "../editor.h"

extern int wvisible(WINDOW *win, pos_t dpos);
extern void editor_printc(pos_t apos);
extern void editor_printline(int aline);
extern void editor_printfile(void);
extern void editor_printborder(void);
extern void editor_printinfo(void);
extern void editor_refresh(void);

#endif
