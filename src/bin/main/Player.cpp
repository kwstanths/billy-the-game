#include "Player.hpp"

#include "game_engine/graphics/Light.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Matrices.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/InteractableObject.hpp"

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
    Scale(0.6, 0.6, 0.6);

    Vector2D pos({ x, y });
    SetCollision(world->GetPhysicsEngine(), AABox<2>(pos - 0.3, pos + 0.3));

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
    /* TODO Remove these checks maybe? */
    if (!is_inited_) return;
    if (!WorldObject::IsInited())
        dt::Console(dt::WARNING, "Player::Step(): WorldObject is not initialised");

    /* Get input */
    ControlInput_t controls = input_->GetControls();

    /* Move player and camera */
    {
        ge::Real_t move_offset = (1.0f * GetSpeed(controls.RUN_)) * static_cast<float>(delta_time);
        /* Find the moving direction based on the input */
        size_t lookup_index = controls.MOVE_UP_ * 8 + controls.MOVE_DOWN_ * 4 + controls.MOVE_LEFT_ * 2 + controls.MOVE_RIGHT_ * 1;
        ge::Real_t new_direction = direction_array_[lookup_index];
        if (!math::Equal(new_direction, -1.0f)) {

            //dt::ConsoleInfo("offset", move_offset, "delta", delta_time * 1000);
            /* Set the rotation of the model */
            SetRotation(ge::math::GetRotateMatrix(math::GetRadians(new_direction), 0, 0, 1));
            looking_direction_ = new_direction;

            /* Set position */
            ge::Real_t pos_x = GetX();
            ge::Real_t pos_y = GetY();
            pos_x = pos_x - controls.MOVE_LEFT_ * move_offset + controls.MOVE_RIGHT_ * move_offset;
            pos_y = pos_y + controls.MOVE_UP_ * move_offset - controls.MOVE_DOWN_ * move_offset;
            SetPosition(pos_x, pos_y, GetZ(), true);
            camera->Set2DPosition(GetX(), GetY());
        }
    }

    /* If interact button is pressed, perform ray casting, and interact with an object in the world */
    if (controls.INTERACT_) {
        Ray2D ray(Vector2D({ GetX(), GetY() }), looking_direction_);

        game_engine::Interactablebject * object = world_sector_->RayCast(ray);
        if (object != nullptr) object->Interact();
    }
}

ge::Real_t Player::GetSpeed(bool running) {
    if (running) return speed_running_;
    else return speed_regular_;
}

void Player::Draw(grph::Renderer * renderer) {

    /* Get input */
    ControlInput_t controls = input_->GetControls();

    grph::Attenuation_t att = ge::graphics::Attenuation_t(1, 0.22f, 0.0009f);
    grph::SpotLight light(glm::vec3(GetX(), GetY(), GetZ() + 5), glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), att, 50.0f, 55.0f);

    if (controls.FLASHLIGHT_) light = grph::SpotLight(glm::vec3(GetX(), GetY(), GetZ() + 5), glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.8, 0.8, 0.8), att, 50.0f, 55.0f);

    renderer->AddSpotLight(&light);
    
    WorldObject::Draw(renderer);
}

void Player::OnCollisionDetected(size_t type) {
    /* Not used, collision and physics need to be reworked */
    if (type == 2) {
        SetPosition(0, 0, GetZ());
        camera_->Set2DPosition(0, 0);
    }

}



