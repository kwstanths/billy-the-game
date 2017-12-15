#include "Player.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;

Player::Player(): WorldObject() {
    is_inited_ = false;
}

int Player::Init() {

    is_inited_ = true;
    return 0;
}

int Player::Destroy() {

    return 0;
}

void Player::Move(float move_x, float move_y) {
    if (!is_inited_) return;

    if (!WorldObject::IsInited()) 
        dt::Console(dt::WARNING, "Player::Move() : WorldObject is not initialised");

    pos_x_ += move_x;
    pos_y_ += move_y;
    
    OpenGLObject::SetPosition(pos_x_, pos_y_, pos_z_);
}

void Player::Step(double delta_time) {

    dt::ConsoleInfoL(dt::INFO, "Player info",
        "delta time", delta_time,
        "x", pos_x_,
        "y", pos_y_,
        "z", pos_z_);

}

