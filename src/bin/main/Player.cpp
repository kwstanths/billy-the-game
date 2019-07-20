#include "Player.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

#include "Wall.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace math = game_engine::math;

Player::Player(): WorldObject() {
    speed_regular_ = 4.0f;
    speed_running_ = 9.0f;
    
    engine_ = nullptr;


    is_inited_ = false;
}

int Player::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, Input * input, Camera * camera, ge::WorldSector * world, ge::GameEngine * engine) {

    input_ = input;
    engine_ = engine;
    camera_ = camera;

    int ret = WorldObject::Init("assets/player.obj", x, y, z);
    world->AddObject(this, x, y, z);
    Scale(0.8, 0.8, 0.8);

    radius_ = 0.5f;
    interact_fov_ = math::GetRadians(50.0f);
    interact_margin_ = 0.3f;
    looking_direction_ = 0.0f;

    camera_->Set2DPosition(GetX(), GetY());

    is_inited_ = true;
    return ret == 0;
}

int Player::Destroy() {

    /* TODO */

    return 0;
}

void Player::Step(double delta_time) {
    /* TODO Remove these checks maybe */
    if (!is_inited_) return;

    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::MovePositionVector(): WorldObject is not initialised");

    /* Get input */
    ControlInput_t controls = input_->GetControls();

    /* Move player and camera */
    {
        ge::Real_t move_offset = (1.0f * GetSpeed(controls.RUN_)) * static_cast<float>(delta_time);
        /* Find the moving direction based on the input */
        size_t lookup_index = controls.MOVE_UP_ * 8 + controls.MOVE_DOWN_ * 4 + controls.MOVE_LEFT_ * 2 + controls.MOVE_RIGHT_ * 1;
        ge::Direction_t new_direction = direction_array_[lookup_index];
        if (!math::Equal(new_direction, -1.0f)) {

            //dt::ConsoleInfo("offset", move_offset, "delta", delta_time * 1000);
            /* Set the rotation of the model */
            SetRotation(math::GetRadians(new_direction), { 0,0,1 });
            looking_direction_ = ge::math::GetRadians(new_direction);

            /* Set position */
            ge::Real_t pos_x = GetX();
            ge::Real_t pos_y = GetY();
            ge::Real_t pos_z = GetZ();
            pos_x = pos_x - controls.MOVE_LEFT_ * move_offset + controls.MOVE_RIGHT_ * move_offset;
            pos_y = pos_y + controls.MOVE_UP_ * move_offset - controls.MOVE_DOWN_ * move_offset;
            SetPosition(pos_x, pos_y, pos_z);
            camera->Set2DPosition(GetX(), GetY());
        }
    }

    /* Interact with objects */
    {
        if (controls.INTERACT_PRESSED || controls.INTERACT_) {
            ge::Direction_t direction = looking_direction_;
            ge::Real_t x1 = GetX() - (radius_ + interact_margin_) * sin(direction + interact_fov_);
            ge::Real_t y1 = GetY() + (radius_ + interact_margin_) * cos(direction + interact_fov_);
            math::Point2D A(x1, y1);

            ge::Real_t x2 = GetX() - (radius_ + interact_margin_) * sin(direction - interact_fov_);
            ge::Real_t y2 = GetY() + (radius_ + interact_margin_) * cos(direction - interact_fov_);
            math::Point2D B(x2, y2);

            ge::Real_t side_size = 1.5f * (radius_ + interact_margin_) * tan(interact_fov_);

            ge::Real_t x3 = x2 - side_size * sin(direction);
            ge::Real_t y3 = y2 + side_size * cos(direction);
            math::Point2D C(x3, y3);

            ge::Real_t x4 = x1 - side_size * sin(direction);
            ge::Real_t y4 = y1 + side_size * cos(direction);
            math::Point2D D(x4, y4);

            engine_->GetDebugger()->DrawPoint(x1, y1, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x2, y2, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x3, y3, 0.5f, 0.08f);
            engine_->GetDebugger()->DrawPoint(x4, y4, 0.5f, 0.08f);

            if (controls.INTERACT_) {
                math::Rectangle2D search_area(A, B, C, D);
                WorldObject * neighbour = world_sector_->FindInteractNeighbour(search_area, math::Point2D((x1 + x2) / 2, (y1 + y2) / 2), 1);
                if (neighbour != nullptr) neighbour->Interact();
                else {

                    /* Spawn new wall! Just for fun! */
                    //{
                    //    /*
                    //        Spawing with normal malloc allocation.
                    //        ATTENTION you will need to deallocate the object yourself orelse memory leak
                    //    */
                    //    Wall * wall = new Wall();
                    //    wall->Init((x1 + x3) / 2, (y1 + y3) / 2, 0.01f, world_sector_, engine_);
                    //}

                    {

                        /* Spawn via the memory system of the game engine */
                        world_sector_->NewObj<Wall>(true)->Init((x1 + x3) / 2, (y1 + y3) / 2, 0.01f, world_sector_, engine_);
                    }
                }
            }
        }
    }

}

ge::Real_t Player::GetSpeed(bool running) {
    if (running) return speed_running_;
    else return speed_regular_;
}

void Player::Draw(grph::Renderer * renderer) {

    /* Get input */
    ControlInput_t controls = input_->GetControls();

    grph::LightProperties_t light(0);
    if (controls.FLASHLIGHT_) light = grph::LightProperties_t(glm::vec3(0, 0, 0), glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.8, 0.8, 0.8));
    grph::Attenuation_t att = ge::graphics::Attenuation_t(1, 0.22f, 0.0009f);
    
    renderer->AddSpotLight(glm::vec3(GetX(), GetY(), GetZ() + 5), glm::vec3(0, 0, -1), 50.0f, 55.0f,
        light, att);
    
    WorldObject::Draw(renderer);
}

void Player::OnCollisionDetected(size_t type) {

    if (type == 2) {
        SetPosition(0, 0, GetZ());
        camera_->Set2DPosition(0, 0);
    }

}



