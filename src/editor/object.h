#ifndef EDITOR_OBJECT_H
#define EDITOR_OBJECT_H

#include "../editor.h"

extern void editor_new(WINDOW *frame);
extern void editor_free(void);

extern void init_ncurses(void);
extern void end_ncurses(void);

#endif
