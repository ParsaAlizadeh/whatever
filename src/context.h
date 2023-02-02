#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdlib.h>
#include "command.h"

#define BUFFER_PATH "/tmp/.buffer"

typedef enum {
    CTX_NULL,
    CTX_BUFFER,
    CTX_PATH
} ctx_mode_t;

extern ctx_mode_t ctx_mode;
extern int ctx_counter;

const char *ctx_get(void);
void ctx_set(ctx_mode_t mode, const char *path);
void ctx_fix(void);

extern const char *ctx_edmode;

extern int ctx_ncmds;
extern const command *ctx_allcmds;

#endif
