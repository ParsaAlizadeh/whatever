#ifndef COMMAND_H
#define COMMAND_H

#include "vector.h"

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
int procedure_command(int, const command [], vector *);

#endif