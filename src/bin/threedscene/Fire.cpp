#include "Fire.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/debug.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    Scale(0.1f, 0.1f, 0.1f);

    PointLight::position_ = glm::vec3(x, y, z);
    PointLight::ambient_ = glm::vec3(0.3f, 0.3f, 0.3f);
    PointLight::diffuse_ = glm::vec3(0.7f, 0.7f, 0.7f);
    PointLight::specular_ = glm::vec3(0.3f, 0.3f, 0.3f);
    PointLight::attenutation_ = ge::graphics::Attenuation_t(1, 0.0001f, 0.000939f);

    world_sector_->AddPointLight(this, math::Point2D({ x, y }));

    return ret == 0;
}

void Fire::Step(double delta_time) {

}

void Fire::Draw(grph::Renderer * renderer) {

    WorldObject::Draw(renderer);
}
