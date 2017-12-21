#include "Player.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/HelpFunctions.hpp"

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
    
    SetPosition(pos_x_, pos_y_, pos_z_);
    if (move_x > 0) Rotate(-game_engine::GetRadians(90.0f), 2);
    else if (move_x < 0) Rotate(-game_engine::GetRadians(-90.0f), 2);
    else if (move_y < 0) Rotate(-game_engine::GetRadians(180.0f), 2);
    else if (move_y > 0) Rotate(0, 2);
}

void Player::Step(double delta_time) {

    //dt::ConsoleInfoL(dt::INFO, "Player info",
    //    "delta time", delta_time,
    //    "x", pos_x_,
    //    "y", pos_y_,
    //    "z", pos_z_);

}

float Player::GetSpeed(bool running) {
    if (!running) return 4.0;
    else return 9.0;
}

