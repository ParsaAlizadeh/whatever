#include "setup.h"

int cmdlog(const command *cmd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size = strlen(cmd->name) + 2 + strlen(format) + 1;
    char *full_format = malloc(size+1);
    sprintf(full_format, "%s: %s\n", cmd->name, format);
    int r = vfprintf(stderr, full_format, args);
    free(full_format);
    va_end(args);
    return r;
}

int cmdlogrequired(const command *cmd, char c) {
    return cmdlog(cmd, "option \"-%c\" is required", c);
}
