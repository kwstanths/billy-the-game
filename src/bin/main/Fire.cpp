#include "Fire.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, std::string name, ge::WorldSector * world, ge::GameEngine * engine, Sun * sun) {

    tile_name = name;

    int ret = WorldObject::Init("assets/" + name + ".obj", x, y, z);
    world->AddObject(this, x, y, z);

    light_.position_ = glm::vec3(x + 0.05, y, z +  0.5);
    light_.properties_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    light_.attenutation_ = ge::graphics::Attenuation_t(1, 0.0001f, 0.00939f);

    attenutation_noise_ = std::vector<ge::Real_t>(201);
    math::RNGenerator gen;
    gen.Init(0.3f);
    gen.GetPerlinNoise1d(201, 0.2f, 0.15f, 70, attenutation_noise_);
    index_ = 0;

    sun_ = sun;

    on_ = true;
    world_sector_->AddLight(&light_, math::Point2D({ x, y }));

    return ret == 0;
}

void Fire::Step(double delta_time) {
    
    index_ = 100 + 100 * sin(glfwGetTime() * 0.5);
    light_.attenutation_.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    /* If dark enough, then set the light up */
    double hour = sun_->GetTimeOfDay();
    if (hour < 8 || hour > 18.5)
        light_.properties_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    else 
        light_.properties_ = ge::graphics::LightProperties_t(0);

}

void Fire::Interact() {

    /* We can just set the light values to zero, but let's just test are RemoveLight function! */
    if (on_) world_sector_->RemoveLight(&light_, math::Point2D({ GetX(), GetY() }));
    else world_sector_->AddLight(&light_, math::Point2D({ GetX(), GetY() }));

    on_ = !on_;
}

