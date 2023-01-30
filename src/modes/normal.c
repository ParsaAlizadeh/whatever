#include "normal.h"

#include <stdlib.h>
#include "movement.h"
#include "insert.h"
#include "visual.h"
#include "command.h"
#include "search.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"

void normal_mode(void) {
    init_ncurses();
    int quits = 0;
    int chr;
    while (1) {
        ctx_set_edmode("NORMAL");
        editor_refresh();
        chr = getch();

        if (chr == 'q') {
            loginfo("q");
            if (++quits >= 2)
                break;
            continue;
        } else {
            quits = 0;
        }

        switch (chr) {
        CHAR_MOVEMENT
        NONCHAR_MOVEMENT
        case 'a':
            insert_mode();
            break;
        case 'v':
            visual_mode();
            break;
        case ':':
            command_mode();
            break;
        case '/':
            search_mode();
            break;
        case KEY_F(1):
            wclear(ed->cw);
            break;
        case '=':
            loginfo("=");
            editor_runf("pretty");
            break;
        case 'u':
            loginfo("u");
            editor_runf("undo");
            break;
        case 'p':
            loginfo("p");
            editor_runf("paste -p %d:%d", ed->acur.line+1, ed->acur.col);
            break;
        case 'n':
            loginfo("n");
            if (editor_hl_next() == -1)
                loginfo("not any highlights available");
            break;
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    end_ncurses();
}
