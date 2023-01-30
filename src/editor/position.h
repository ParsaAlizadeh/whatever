#ifndef EDITOR_POSITION_H
#define EDITOR_POSITION_H

#include "../editor.h"

int editor_dline(pos_t apos);
int editor_dcol(pos_t apos);
pos_t editor_dpos(pos_t apos);
int editor_acol(int aline, int dcol);

#endif
