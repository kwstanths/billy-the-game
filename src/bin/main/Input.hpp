#ifndef __Input_hpp__
#define __Input_hpp__

#include "game_engine/GameEngine.hpp"

/**
    Control input
*/
typedef struct {
    bool QUIT_ = false;

    bool MOVE_LEFT_ = false;
    bool MOVE_RIGHT_ = false;
    bool MOVE_UP_ = false;
    bool MOVE_DOWN_ = false;

    bool RUN_ = false;
    bool INTERACT_ = false;
    bool INTERACT_PRESSED = false;

    bool ZOOM_IN_ = false;
    bool ZOOM_OUT_ = false;

} ControlInput_t;

class Input {
public:
    Input();

    int Init(game_engine::GameEngine * engine);

    int Destroy();

    bool IsInited();

    ControlInput_t GetControls();

private:
    double last_control_timestamp_;
    bool is_inited_, previous_interact_state_;

    ControlInput_t control_input_;

    game_engine::GameEngine * engine_;

};


#endif
