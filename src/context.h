#ifndef CONTEXT_H
#define CONTEXT_H

#include "command.h"

#define BUFFER_PATH "/tmp/.buffer"

const char *ctx_get(void);
void ctx_set(const char *path);
void ctx_clear(void);
void ctx_save(void);

int ctx_get_buf_mode(void);
void ctx_set_buf_mode(int mode);

const char *ctx_get_edmode(void);
void ctx_set_edmode(const char *edmode);

void ctx_set_cmds(int n_cmds, const command *all_cmds);
int ctx_get_ncmds(void);
const command *ctx_get_allcmds(void);

#endif
