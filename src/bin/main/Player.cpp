#include "Player.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/physics/HelpFunctions.hpp"
#include "game_engine/physics/Geometry.hpp"

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

    bool moving_up = !game_engine::Equal(collision_input.up_, 0.0f);
    bool moving_down = !game_engine::Equal(collision_input.down_, 0.0f);
    bool moving_left = !game_engine::Equal(collision_input.left_, 0.0f);
    bool moving_right = !game_engine::Equal(collision_input.right_, 0.0f);

    /* Move player */
    float pos_x = GetXPosition();
    float pos_y = GetYPosition();
    float pos_z = GetZPosition();
    if (moving_left) pos_x -= collision_input.left_;
    if (moving_right) pos_x += collision_input.right_;
    if (moving_up) pos_y += collision_input.up_;
    if (moving_down) pos_y += -collision_input.down_;
    SetPosition(pos_x, pos_y, pos_z);

    /* Rotate player if we are moving */
    if (moving_left && moving_up) Rotate(game_engine::GetRadians(45.0f), 2);
    else if (moving_left && moving_down) Rotate(game_engine::GetRadians(135.0f), 2);
    else if (moving_right && moving_up) Rotate(game_engine::GetRadians(-45.0f), 2);
    else if (moving_right && moving_down) Rotate(game_engine::GetRadians(-135.0f), 2);
    else if (moving_up) Rotate(game_engine::GetRadians(0), 2);
    else if (moving_left) Rotate(game_engine::GetRadians(90.0f), 2);
    else if (moving_right) Rotate(game_engine::GetRadians(-90.0f), 2);
    else if (moving_down) Rotate(game_engine::GetRadians(180.0f), 2);

    /* Rotate player if we cannot move but we must rotate */
    if (control_input.KEY_UP && !moving_up) Rotate(game_engine::GetRadians(0), 2);
    else if (control_input.KEY_LEFT && !moving_left) Rotate(game_engine::GetRadians(90.0f), 2);
    else if (control_input.KEY_RIGHT && !moving_right) Rotate(game_engine::GetRadians(-90.0f), 2);
    else if (control_input.KEY_DOWN && !moving_down) Rotate(game_engine::GetRadians(180.0f), 2);
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


