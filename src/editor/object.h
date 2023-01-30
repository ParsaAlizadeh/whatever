#ifndef EDITOR_OBJECT_H
#define EDITOR_OBJECT_H

#include "../editor.h"

void editor_new(WINDOW *frame);
void editor_free(void);

void init_ncurses(void);
void end_ncurses(void);

#endif
