#include "Player.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/physics/HelpFunctions.hpp"
#include "game_engine/physics/Geometry.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include "Wall.hpp"

namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

Player::Player(): WorldObject() {
    speed_regular_ = 4.0f;
    speed_running_ = 9.0f;
    
    engine_ = nullptr;

    is_inited_ = false;
}

int Player::Init(float x, float y, float z, Input * input, Camera * camera, ge::GameEngine * engine) {

    input_ = input;
    engine_ = engine;
    camera_ = camera;

    int ret;
    gl::OpenGLObject * object = engine_->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    gl::OpenGLTexture * texture = engine_->GetAssetManager()->FindTexture("assets/player.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture, x, y, z);
    radius_ = 0.5f;
    interact_fov_ = ge::GetRadians(50.0f);
    interact_margin_ = 0.2f;

    /* Scale object down to half */
    Scale(0.5f, 0.5f, 1.0f);
    radius_ = radius_ * 0.5f;
    SetCollision(radius_);

    ge::graphics::Material_t m(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.85, 0.70, 0.50), glm::vec3(0.1, 0.1, 0.1), 32);
    SetMaterial(m);

    is_inited_ = true;
    return ret == 0;
}

int Player::Destroy() {

    /* TODO */

    return 0;
}

void Player::Move(float move_offset, game_engine::CollisionResult_t collision_input) {
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::MovePositionVector(): WorldObject is not initialised");

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
}

void Player::Step(double delta_time) {
    /* TODO Remove these checks maybe */
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::MovePositionVector(): WorldObject is not initialised");

    /* Get input */
    ControlInput_t controls = input_->GetControls();

    /* Interact with objects */
    {
        if (controls.INTERACT_PRESSED || controls.INTERACT_) {
            ge::Direction direction = GetLookingDirection();
            float x1 = GetX() - (radius_ + interact_margin_) * sin(direction + interact_fov_);
            float y1 = GetY() + (radius_ + interact_margin_) * cos(direction + interact_fov_);
            ge::Point2D A(x1, y1);

            float x2 = GetX() - (radius_ + interact_margin_) * sin(direction - interact_fov_);
            float y2 = GetY() + (radius_ + interact_margin_) * cos(direction - interact_fov_);
            ge::Point2D B(x2, y2);

            float side_size = 1.5f * (radius_ + interact_margin_) * tan(interact_fov_);

            float x3 = x2 - side_size * sin(direction);
            float y3 = y2 + side_size * cos(direction);
            ge::Point2D C(x3, y3);

            float x4 = x1 - side_size * sin(direction);
            float y4 = y1 + side_size * cos(direction);
            ge::Point2D D(x4, y4);

            engine_->GetDebugger()->DrawPoint(x1, y1, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x2, y2, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x3, y3, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x4, y4, 0.5f, 0.08f);

            if (controls.INTERACT_) {
                ge::Rectangle2D search_area(A, B, C, D);
                WorldObject * neighbour = world_sector_->FindNeighbour(search_area, GetX(), GetY());
                if (neighbour != nullptr) neighbour->Interact();
                else {
                    /* Spawn new wall! Just for fun! */
                    world_sector_->NewObj<Wall>(true)->Init((x3 + x4) / 2, (y3 + y4) / 2, 0.01f, engine_);
                }
            }
        }
    }

    /* Move player and camera */
    {
        float move_offset = (1.0f * GetSpeed(controls.RUN_)) * delta_time;
        /* Find the moving direction based on the input */
        size_t lookup_index = controls.MOVE_UP_ * 8 + controls.MOVE_DOWN_ * 4 + controls.MOVE_LEFT_ * 2 + controls.MOVE_RIGHT_ * 1;
        ge::Direction direction = direction_array_[lookup_index];
        if (!ge::Equal(direction, -1.0f)) {

            /* Rotate*/
            Rotate(ge::GetRadians(direction), 2);

            /* Check for collision and Move player and camera */
            ge::CollisionResult_t can_move = CheckCollision(move_offset, direction);
            Move(move_offset, can_move);

            camera_->KeyboardMove(can_move.right_ - can_move.left_, can_move.up_ - can_move.down_, 0);
        }
    }

}

float Player::GetSpeed(bool running) {
    if (running) return speed_running_;
    else return speed_regular_;
}

void Player::Draw(grph::Renderer * renderer) {

    //renderer->AddLight(glm::vec3(GetX() + 4*sin(glfwGetTime()), GetY() + 4*cos(glfwGetTime()), GetZ() + 3) , glm::vec3(255.0 / 255.0, 214.0 / 255.0, 170.0 / 255.0));

    WorldObject::Draw(renderer);
}


