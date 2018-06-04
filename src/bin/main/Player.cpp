#include "Player.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/physics/HelpFunctions.hpp"
#include "game_engine/physics/Geometry.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;

Player::Player(): WorldObject() {
    speed_regular_ = 4.0f;
    speed_running_ = 9.0f;
    
    engine_ = nullptr;

    is_inited_ = false;
}

int Player::Init(float x, float y, float z, ge::GameEngine * engine) {

    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/player.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture, x, y, z);

    SetCollision(0.5);

    engine_ = engine;

    is_inited_ = true;
    return ret == 0;
}

int Player::Destroy() {

    /* TODO */

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
    float pos_x = GetX();
    float pos_y = GetY();
    float pos_z = GetZ();
    pos_x -= collision_input.left_;
    pos_x += collision_input.right_;
    pos_y += collision_input.up_;
    pos_y += -collision_input.down_;
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
    /* TODO Remove these checks maybe */
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::Move(): WorldObject is not initialised");

    /* Get input */
    ControlInput input = engine_->GetControlsInput();

    /* Move player and camera */
    float move_offset = (1.0f * GetSpeed(input.KEY_RUN)) * delta_time;
    /* Find the moving direction based on the input */
    size_t lookup_index = input.KEY_UP * 8 + input.KEY_DOWN * 4 + input.KEY_LEFT * 2 + input.KEY_RIGHT * 1;
    float direction_array_[16] = { -1, 270, 90, -1, 180, 225, 135, -1, 0, 345, 45, -1, -1, -1, -1, -1 };
    ge::Direction direction = direction_array_[lookup_index];
    if (ge::Equal(direction, -1.0f)) return;
    /* Check for collision and move */
    ge::CollisionResult_t can_move = CheckCollision(move_offset, direction);
    Move(move_offset, input, can_move);
    if (can_move.left_) engine_->CameraMove2D(-can_move.left_, 0);
    if (can_move.right_) engine_->CameraMove2D(can_move.right_, 0);
    if (can_move.up_) engine_->CameraMove2D(0, can_move.up_);
    if (can_move.down_) engine_->CameraMove2D(0, -can_move.down_);
}

float Player::GetSpeed(bool running) {
    if (running) return speed_running_;
    else return speed_regular_;
}


