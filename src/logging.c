#include "logging.h"

#include "editor.h"

int vloginfo(const char *format, va_list ap) {
    va_list ap2;
    va_copy(ap2, ap);
    int r = vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    if (ed != NULL) {
        wclear(ed->cw);
        wmove(ed->cw, 0, 0);
        vw_printw(ed->cw, format, ap2);
        wrefresh(ed->cw);
        editor_mvcur();
    }
    va_end(ap2);
    return r;
}

int loginfo(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int r = vloginfo(format, args);
    va_end(args);
    return r;
}
