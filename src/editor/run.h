#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "../editor.h"

typedef enum {
    RUN_READONLY,
    RUN_READWRITE
} run_mode_t;

void editor_run_init(void);
void editor_run_end(run_mode_t mode, char *out);
void editor_run(run_mode_t mode, vector *tokens);
void editor_runf(run_mode_t mode, const char *format, ...);

#endif
