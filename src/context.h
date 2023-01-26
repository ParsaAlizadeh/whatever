#ifndef CONTEXT_H
#define CONTEXT_H

#define BUFFER_PATH "/tmp/.buffer"

const char *ctx_get(void);
void ctx_set(const char *path);
void ctx_clear(void);
void ctx_save(void);

int ctx_get_buf_mode(void);
void ctx_set_buf_mode(int mode);

#endif
