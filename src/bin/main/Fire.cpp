#include "Fire.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, game_engine::GameEngine * engine, Sun * sun) {

    int ret = WorldObject::Init("assets/debug.obj", x, y, z, true);

    Scale(0.1f, 0.1f, 0.1f);
    SetCollision(math::Circle2D(x, y, 0.05f));

    light_.position_ = glm::vec3(x, y, z + 0.5);
    light_.properties_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    light_.attenutation_ = ge::graphics::Attenuation_t(1, 0.01f, 0.0239f);

    attenutation_noise_ = std::vector<ge::Real_t>(201);
    math::RNGenerator gen;
    gen.Init(0.3f);
    gen.GetPerlinNoise1d(201, 0.2f, 0.15f, 70, attenutation_noise_);
    index_ = 0;

    sun_ = sun;

    on_ = true;
    world_sector_->AddLight(&light_, math::Point2D(x, y));

    return ret == 0;
}

void Fire::Step(double delta_time) {
    
    light_.attenutation_.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    /* If dark enough, then set the light up */
    double hour = sun_->GetTimeOfDay();
    if (hour < 8 || hour > 18.5)
        light_.properties_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    else 
        light_.properties_ = ge::graphics::LightProperties_t(0);

}

void Fire::Draw(grph::Renderer * renderer) {
    
    WorldObject::Draw(renderer);
}

void Fire::Interact() {

    /* We can just set the light values to zero, but let's just test are RemoveLight function! */
    if (on_) world_sector_->RemoveLight(&light_, math::Point2D(GetX(), GetY()));
    else world_sector_->AddLight(&light_, math::Point2D(GetX(), GetY()));

    on_ = !on_;
}

