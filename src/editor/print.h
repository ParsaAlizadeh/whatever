#ifndef EDITOR_PRINT_H
#define EDITOR_PRINT_H

#include "../editor.h"

int wvisible(WINDOW *win, pos_t dpos);
void editor_printc(pos_t apos);
void editor_printline(int aline);
void editor_printfile(void);
void editor_printborder(void);
void editor_printinfo(void);
void editor_refresh(void);

#endif
