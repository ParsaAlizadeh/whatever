#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "../editor.h"

extern void editor_reset(void);
extern void editor_setvc(vecline *vc);
extern int editor_readstr(char *str);
extern int editor_readpath(const char *path);
extern int editor_writepath(const char *path);
extern int editor_readctx(void);
extern int editor_writectx(void);
extern void editor_unlinkbuffer(void);

#endif
