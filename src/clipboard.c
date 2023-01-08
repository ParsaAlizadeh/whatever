#include "clipboard.h"

#include <stdlib.h>
#include <string.h>
#include "vecstr.h"

static string *clipboard = NULL;

void clipboard_set(char *s) {
    if (clipboard != NULL)
        free(string_free(clipboard));
    clipboard = string_from(s);
}

const char *clipboard_get(void) {
    if (clipboard == NULL)
        return "";
    return clipboard->seq;
}