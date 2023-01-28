#include "normal.h"

#include <stdlib.h>
#include "movement.h"
#include "insert.h"
#include "visual.h"
#include "command.h"
#include "../logging.h"
#include "../editor.h"
#include "../context.h"
#include "../parse.h"

void normal_mode(int n_cmds, const command all_cmds[]) {
    (void) n_cmds;
    (void) all_cmds;
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
        HOMEEND_MOVEMENT
        case 'a':
            insert_mode();
            break;
        case 'v':
            visual_mode();
            break;
        case ':':
            command_mode(n_cmds, all_cmds);
            break;
        case KEY_F(1):
            wclear(ed->cw);
            break;
        case '=':
            loginfo("=");
            editor_run_command(n_cmds, all_cmds, scan_strline("pretty"));
            break;
        default:
            loginfo("undefined key: %d", chr);
        }
    }
    end_ncurses();
}
