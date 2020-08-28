#include "Fire.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, float intensity, ge::WorldSector * world, ge::GameEngine * engine, Sun * sun) {

    diffuse_value_ = glm::vec3(0.9f, 0.6f, 0.6f) * intensity;

    PointLight::position_ = glm::vec3(x + 0.05, y, z +  1.5);
    PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    PointLight::diffuse_ = diffuse_value_;
    PointLight::specular_ = glm::vec3(0.0f, 0.0f, 0.0f);
    PointLight::attenutation_ = ge::graphics::Attenuation_t(1, 0.0001f, 0.0939f);

    attenutation_noise_ = std::vector<ge::Real_t>(201);
    math::RNGenerator gen;
    gen.GetPerlinNoise1d(201, 0.2f, 0.5f, 70, attenutation_noise_);
    index_ = rand() % 199;

    sun_ = sun;

    on_ = true;
    int ret = world->AddPointLight(this, math::Vector2D({ x, y }));
    ret = world->AddInterractableObject(this, math::AABox<2>(Vector2D({ x, y }), { 1,1 }));
    return ret == 0;
}

void Fire::StepLight(double delta_time) {
    /* Change attenuation based on the perlin noise generated */
    PointLight::attenutation_.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    if (sun_ != nullptr) {
        /* If dark enough, then set the light up */
        double hour = sun_->GetTimeOfDay();
        if (on_ && (hour < 8 || hour > 18.5)) {
            PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
            PointLight::diffuse_ = diffuse_value_;
            PointLight::specular_ = glm::vec3(0.4f, 0.4f, 0.4f);
        } else {
            PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
            PointLight::diffuse_ = glm::vec3(0.0f, 0.0f, 0.0f);
            PointLight::specular_ = glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }

}

void Fire::Interact()
{
    /* Turn on or off the light */
    if (on_) PointLight::diffuse_ = glm::vec3(0.0f, 0.0f, 0.0f);
    else PointLight::diffuse_ = glm::vec3(0.7f, 0.5f, 0.5f);

    on_ = !on_;
}

