#ifndef __Controls_hpp__
#define __Controls_hpp__

namespace game_engine {

    typedef struct KeyControls {
        bool KEY_ESC = false;
        bool KEY_ENTER = false;
        bool KEY_LSHIFT = false;
        bool KEY_LALT = false;

        bool KEY_A = false;
        bool KEY_S = false;
        bool KEY_D = false;
        bool KEY_W = false;
        bool KEY_E = false;
        bool KEY_F = false;

        bool KEY_O = false;
        bool KEY_R = false;
        bool KEY_V = false;

        bool KEY_ARROW_UP = false;
        bool KEY_ARROW_DOWN = false;
        bool KEY_ARROW_LEFT = false;
        bool KEY_ARROW_RIGHT = false;

        bool KEY_PAGE_UP = false;
        bool KEY_PAGE_DOWN = false;

        double timestamp_;
    } KeyControls_t;

}

#endif