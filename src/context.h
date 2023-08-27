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

extern const char *ctx_get(void);
extern void ctx_set(ctx_mode_t mode, const char *path);
extern void ctx_fix(void);

extern const char *ctx_edmode;

extern int ctx_ncmds;
extern const command *ctx_allcmds;

#endif
