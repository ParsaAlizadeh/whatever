#ifndef CBOXI_H
#define CBOXI_H

#include <stdio.h>

typedef enum {
    CBOXI_CONTINUE,
    CBOXI_CANCEL,
    CBOXI_DONE
} cboxi_rc_t;

extern void cboxi_init(char c);
extern void cboxi_end(void);
extern cboxi_rc_t cboxi_refresh(void);
extern const char *cboxi_content(void);
extern FILE *cboxi_fcontent(void);
extern int cboxi_size(void);

#endif
