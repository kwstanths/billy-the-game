#include "Sun.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace dt = debug_tools;

bool Sun::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/circle.obj", x, y, z);
    world->AddObject(this, x, y, z, true);

    light_ = ge::graphics::LightProperties_t(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    //light_ = ge::graphics::LightProperties_t(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    /* Normal frequency of one day in seconds */
    day_period_ = 86400.0;

    return ret == 0;
}

void Sun::Step(double delta_time) {
    
    /* Time passes 400 times faster in the game */
    game_time_ += 2000.0f * static_cast<float>(delta_time);
    if (game_time_ > day_period_) game_time_ = 0;

    /* Calcluate the hour of the game */
    game_hour_ = game_time_ / 3600.0f;
    
    /* 15 degrees for each hour for the 24 hours cycle */
    float color = ge::math::clamp(sin(ge::math::GetRadians((game_hour_ - 7) * 15)), 0.2f, 1.0f);

    light_.diffuse_ = glm::vec3(color, color, color);
    light_.specular_ = glm::vec3(0.5 * color, 0.5 * color, 0.5 * color);
}

void Sun::Draw(grph::Renderer * renderer) {

    renderer->AddDirectionalLight(glm::vec3(0, 0, -1), light_);

    WorldObject::Draw(renderer);
}

double Sun::GetTimeOfDay() {
    return game_hour_;
}

