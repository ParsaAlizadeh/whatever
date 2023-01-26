#ifndef CONTEXT_H
#define CONTEXT_H

const char *ctx_get(void);
void ctx_set(const char *path);
void ctx_clear(void);

#endif
