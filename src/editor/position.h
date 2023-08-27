#ifndef EDITOR_POSITION_H
#define EDITOR_POSITION_H

#include "../editor.h"

extern int editor_dline(pos_t apos);
extern int editor_dcol(pos_t apos);
extern pos_t editor_dpos(pos_t apos);
extern int editor_acol(int aline, int dcol);

#endif
