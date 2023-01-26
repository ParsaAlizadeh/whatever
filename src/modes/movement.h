#define CASE_MOVEMENT(l, d, u, r) \
    case l: \
        loginfo("left"); \
        editor_left(); \
        break; \
    case d: \
        loginfo("down"); \
        editor_down(); \
        break; \
    case u: \
        loginfo("up"); \
        editor_up(); \
        break; \
    case r: \
        loginfo("right"); \
        editor_right(); \
        break; \

#define HOMEEND_MOVEMENT \
    case KEY_HOME: \
        loginfo("home"); \
        editor_home(); \
        break; \
    case KEY_END: \
        loginfo("end"); \
        editor_end(); \
        break;

#define ARROW_MOVEMENT  CASE_MOVEMENT(KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT)
#define VIM_MOVEMENT    CASE_MOVEMENT('h', 'j', 'k', 'l')
