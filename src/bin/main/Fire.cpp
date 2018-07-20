#include "Fire.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/math/RNGenerator.hpp"

namespace ge = game_engine;
namespace mh = game_engine::math;
namespace grph = game_engine::graphics;
namespace dt = debug_tools;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, game_engine::GameEngine * engine, Sun * sun) {

    int ret = WorldObject::Init("assets/debug.obj", x, y, z);

    Scale(0.1f, 0.1f, 0.1f);
    SetCollision(0.05f);

    light_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    att_ = ge::graphics::Attenuation_t(1, 0.02f, 0.0239f);

    attenutation_noise_ = std::vector<ge::Real_t>(201);
    mh::RNGenerator gen;
    gen.Init(0.3f);
    gen.GetPerlinNoise1d(201, 0.2f, 0.15f, 70, attenutation_noise_);
    index_ = 0;

    sun_ = sun;

    return ret == 0;
}

void Fire::Step(double delta_time) {

}

void Fire::Draw(grph::Renderer * renderer) {
    
    double hour = sun_->GetTimeOfDay();

    grph::Attenuation_t att = att_;
    att.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    /* If dark enough, then set the light up */
    if (hour < 8 || hour > 18.5) renderer->AddPointLight(glm::vec3(GetX(), GetY(), GetZ()+0.5), light_, att);
    else renderer->AddPointLight(glm::vec3(GetX(), GetY(), GetZ() + 0.5), ge::graphics::LightProperties_t(0), att_);

    WorldObject::Draw(renderer);
}

