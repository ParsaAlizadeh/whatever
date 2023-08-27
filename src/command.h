#ifndef COMMAND_H
#define COMMAND_H

#include "vector.h"

enum cmd_error {
    CMD_UNEXPECTED = -3,
    CMD_REPEATED_OPTION = -2,
    CMD_FAILURE = -1,
    CMD_SUCCESS = 0
};

typedef struct {
    const char *name;
    const char *optstring;
    const char *usage;
    void *(*make)(void);
    int   (*set_opt)(void *, int, char *);
    void  (*run)(void *, char *, char **);
    void  (*free)(void *);
} command;

typedef struct {
    void *obj;
    const command *cmd;
} command_obj;

extern const command *lookup_command(char *);
extern command_obj make_command(vector *);
extern int feed_options(command_obj, vector *);
extern void run_command(command_obj, char *inp, char **out);

/*
 * given an array of all commands and a vector of tokens,
 * runs the command and return an cmd_error (0 on success)
 */
extern int procedure_command(vector *tokens, char *inp, char **out);

#endif