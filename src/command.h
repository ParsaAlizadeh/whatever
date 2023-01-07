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
    char *name;
    char *optstring;
    void *(*make)(void);
    int   (*set_opt)(void *, int, char *);
    void  (*run)(void *);
} command;

typedef struct {
    void *obj;
    const command *cmd;
} command_obj;

const command *lookup_command(int, const command [], char *);
command_obj make_command(int, const command [], vector *);
int feed_options(command_obj, vector *);
void run_command(command_obj);

/*
 * given an array of all commands and a vector of tokens,
 * runs the command and return an cmd_error (0 on success)
 */
int procedure_command(int, const command [], vector *);

#endif