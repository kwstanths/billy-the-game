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

    PointLight::position_ = glm::vec3(x + 0.05, y, z +  1.5);
    PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    PointLight::diffuse_ = glm::vec3(0.7f, 0.5f, 0.5f); 
    PointLight::specular_ = glm::vec3(0.2f, 0.2f, 0.2f);
    PointLight::attenutation_ = ge::graphics::Attenuation_t(1, 0.0001f, 0.0939f);

    attenutation_noise_ = std::vector<ge::Real_t>(201);
    math::RNGenerator gen;
    gen.Init(0.3f);
    gen.GetPerlinNoise1d(201, 0.2f, 0.15f, 70, attenutation_noise_);
    index_ = 0;

    sun_ = sun;

    on_ = true;
    world_sector_->AddPointLight(this, math::Vector2D({ x, y }));
    world_sector_->AddInterractableObject(this, AABox<2>(Vector2D({ GetX(), GetY() }), { 1,1 }));

    return ret == 0;
}

void Fire::StepLight(double delta_time) {
    
    index_ = 100 + 100 * sin(glfwGetTime() * 0.5);
    PointLight::attenutation_.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    /* If dark enough, then set the light up */
    double hour = sun_->GetTimeOfDay();
    if (hour < 8 || hour > 18.5) {
        PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
        PointLight::diffuse_ = glm::vec3(0.9f, 0.7f, 0.7f);
        PointLight::specular_ = glm::vec3(0.4f, 0.4f, 0.4f);
    } else {
        PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
        PointLight::diffuse_ = glm::vec3(0.0f, 0.0f, 0.0f);
        PointLight::specular_ = glm::vec3(0.0f, 0.0f, 0.0f);
    }

}

void Fire::Interact() {

    /* We can just set the light values to zero, but let's just test the RemoveLight function! */
    if (on_) world_sector_->RemovePointLight(this, math::Vector2D({ GetX(), GetY() }));
    else world_sector_->AddPointLight(this, math::Vector2D({ GetX(), GetY() }));

    on_ = !on_;
}

