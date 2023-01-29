#include "normal.h"

#include <stdlib.h>
#include "movement.h"
#include "insert.h"
#include "visual.h"
#include "command.h"
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
        ARROW_MOVEMENT
        VIM_MOVEMENT
        MISC_MOVEMENT
        case 'a':
            insert_mode();
            break;
        case 'v':
            visual_mode();
            break;
        case ':':
            command_mode();
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
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    end_ncurses();
}
