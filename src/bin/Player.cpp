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

void Player::Move(float move_offset, ControlInput control_input, game_engine::CollisionResult_t collision_input) {
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::Move(): WorldObject is not initialised");

    bool can_move_up = !game_engine::Equal(collision_input.up_, 0.0f);
    bool can_move_down = !game_engine::Equal(collision_input.down_, 0.0f);
    bool can_move_left = !game_engine::Equal(collision_input.left_, 0.0f);
    bool can_move_right = !game_engine::Equal(collision_input.right_, 0.0f);

    /* Move player */
    if (can_move_left) pos_x_ += -collision_input.left_;
    if (can_move_right) pos_x_ += collision_input.right_;
    if (can_move_up) pos_y_ += collision_input.up_;
    if (can_move_down) pos_y_ += -collision_input.down_;
    SetPosition(pos_x_, pos_y_, pos_z_);

    /* Rotate player if we are moving */
    if (can_move_left && can_move_up) Rotate(game_engine::GetRadians(45.0f), 2);
    else if (can_move_left && can_move_down) Rotate(game_engine::GetRadians(135.0f), 2);
    else if (can_move_right && can_move_up) Rotate(game_engine::GetRadians(-45.0f), 2);
    else if (can_move_right && can_move_down) Rotate(game_engine::GetRadians(-135.0f), 2);
    else if (can_move_up) Rotate(game_engine::GetRadians(0), 2);
    else if (can_move_left) Rotate(game_engine::GetRadians(90.0f), 2);
    else if (can_move_right) Rotate(game_engine::GetRadians(-90.0f), 2);
    else if (can_move_down) Rotate(game_engine::GetRadians(180.0f), 2);

    /* Rotate player if we cannot move but we must rotate */
    if (control_input.KEY_UP && !can_move_up) Rotate(game_engine::GetRadians(0), 2);
    else if (control_input.KEY_LEFT && !can_move_left) Rotate(game_engine::GetRadians(90.0f), 2);
    else if (control_input.KEY_RIGHT && !can_move_right) Rotate(game_engine::GetRadians(-90.0f), 2);
    else if (control_input.KEY_DOWN && !can_move_down) Rotate(game_engine::GetRadians(180.0f), 2);
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


