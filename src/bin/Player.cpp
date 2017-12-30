#include "Player.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/HelpFunctions.hpp"

namespace dt = debug_tools;

Player::Player(): WorldObject() {
    speed_regular_ = 4.0f;
    speed_running_ = 9.0f;
    
    is_inited_ = false;
}

int Player::Init() {

    is_inited_ = true;
    return 0;
}

int Player::Destroy() {

    return 0;
}

void Player::Move(float move_offset, game_engine::CollisionResult_t input) {
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::Move(): WorldObject is not initialised");

    /* Move player */
    if (input.move_left) pos_x_ += -move_offset;
    if (input.move_right) pos_x_ += move_offset;
    if (input.move_up) pos_y_ += move_offset;
    if (input.move_down) pos_y_ += -move_offset;
    SetPosition(pos_x_, pos_y_, pos_z_);

    /* Rotate player */
    if (input.move_left && input.move_up) Rotate(game_engine::GetRadians(45.0f), 2);
    else if (input.move_left && input.move_down) Rotate(game_engine::GetRadians(135.0f), 2);
    else if (input.move_right && input.move_up) Rotate(game_engine::GetRadians(-45.0f), 2);
    else if (input.move_right && input.move_down) Rotate(game_engine::GetRadians(-135.0f), 2);
    else if (input.move_up) Rotate(game_engine::GetRadians(0), 2);
    else if (input.move_left) Rotate(game_engine::GetRadians(90.0f), 2);
    else if (input.move_right) Rotate(game_engine::GetRadians(-90.0f), 2);
    else if (input.move_down) Rotate(game_engine::GetRadians(180.0f), 2);
}

void Player::Step(double delta_time) {

    //dt::ConsoleInfoL(dt::INFO, "Player info",
    //    "delta time", delta_time,
    //    "x", pos_x_,
    //    "y", pos_y_,
    //    "z", pos_z_);

}

float Player::GetSpeed(bool running) {
    if (running) return speed_running_;
    else return speed_regular_;
}


