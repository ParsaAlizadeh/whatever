#include "normal.h"

#include <stdlib.h>
#include "../logging.h"
#include "../editor.h"

void normal_mode(int n_cmds, const command all_cmds[]) {
    (void) n_cmds;
    (void) all_cmds;
    init_ncurses();
    int quits = 0;
    int chr;
    while (1) {
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
        case KEY_LEFT:
        case 'h':
            loginfo("left");
            editor_left();
            break;

        case KEY_DOWN:
        case 'j':
            loginfo("down");
            editor_down();
            break;

        case KEY_UP:
        case 'k':
            loginfo("up");
            editor_up();
            break;

        case KEY_RIGHT:
        case 'l':
            loginfo("right");
            editor_right();
            break;

        case KEY_HOME:
            loginfo("home");
            editor_home();
            break;

        case KEY_END:
            loginfo("end");
            editor_end();
            break;

        default:
            loginfo("unkown key");
        }
    }
    end_ncurses();
}
