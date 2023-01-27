#ifndef CBOXI_H
#define CBOXI_H

#include <stdio.h>

typedef enum {
    CBOXI_CONTINUE,
    CBOXI_CANCEL,
    CBOXI_DONE
} cboxi_rc_t;

void cboxi_init(char c);
void cboxi_end(void);
cboxi_rc_t cboxi_refresh(void);
FILE *cboxi_content(void);

#endif
