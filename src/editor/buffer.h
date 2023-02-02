#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "../editor.h"

void editor_reset(void);
void editor_setvc(vecline *vc);
int editor_readstr(char *str);
int editor_readpath(const char *path);
int editor_writepath(const char *path);
int editor_readctx(void);
int editor_writectx(void);
void editor_unlinkbuffer(void);

#endif
