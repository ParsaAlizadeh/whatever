#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "../editor.h"

void editor_reset(void);
void editor_setvc(vecline *vc);
int editor_saveas(const char *path);
int editor_loadctx(void);
void editor_unlinkbuffer(void);

#endif
