#include "Fire.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    PointLight::position_ = glm::vec3(x, y, z);
    PointLight::ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    PointLight::diffuse_ = glm::vec3(0.9f, 0.6f, 0.6f) ;
    PointLight::specular_ = glm::vec3(0.0f, 0.0f, 0.0f);
    PointLight::attenutation_ = ge::graphics::Attenuation_t(1, 0.0001f, 0.0939f);

    int ret = world->AddPointLight(this, math::Vector2D({ x, y }));
    return ret == 0;
}
