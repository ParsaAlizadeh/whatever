#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "../editor.h"

extern void editor_run_init(void);
extern void editor_run_end(char *out);
extern void editor_run(vector *tokens);
extern void editor_runf(const char *format, ...);

#endif
