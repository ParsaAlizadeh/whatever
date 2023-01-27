#ifndef SETUP_H
#define SETUP_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../fileutil.h"
#include "../vecstr.h"
#include "../vector.h"
#include "../command.h"
#include "../clipboard.h"
#include "../parse.h"
#include "../logging.h"
#include "../context.h"
#include "../editor.h"

int cmdlog(const command *, const char *, ...);
int cmdlogrequired(const command *, char);

#define SINGLE_OPTION_CONSTANT(c, memb, def, val) \
    case c: \
        if (this->memb != (def)) \
            return CMD_REPEATED_OPTION; \
        this->memb = (val); \
        break;

#define SINGLE_OPTION_ARGV(c, memb) SINGLE_OPTION_CONSTANT(c, memb, NULL, argv)

#define SINGLE_OPTION_SCANF_COND(c, cmd, memb, def, format, cond) \
    case c: \
        if (this->memb != (def)) \
            return CMD_REPEATED_OPTION; \
        if (sscanf(argv, format, &this->memb) < 1) { \
            cmdlog(&cmd, "option \"-%c\" must be in this format %s", c, #format); \
            return CMD_FAILURE; \
        } \
        if (!(cond)) { \
            cmdlog(&cmd, "invalid option argument \"-%c\"", c); \
            return CMD_FAILURE; \
        } \
        break;

#define SINGLE_OPTION_POSITION(c, cmd, line_no, col_no) \
    case c: \
        if (this->line_no != -1) \
            return CMD_REPEATED_OPTION; \
        if (sscanf(argv, "%ld:%ld", &this->line_no, &this->col_no) < 2) { \
            cmdlog(&cmd, "option \"-%c\" must be a position \"%%ld:%%ld\"", c); \
            return CMD_FAILURE; \
        } \
        if (this->line_no <= 0) { \
            cmdlog(&cmd, "line number must be positive"); \
            return CMD_FAILURE; \
        } \
        if (this->col_no < 0) { \
            cmdlog(&cmd, "column number must be non-negative"); \
            return CMD_FAILURE; \
        } \
        break;

#endif