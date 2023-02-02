#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "../editor.h"

void editor_run_init(void);
void editor_run_end(char *out);
void editor_run(vector *tokens);
void editor_runf(const char *format, ...);

#endif
