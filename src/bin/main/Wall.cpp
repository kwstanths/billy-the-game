#include "Wall.hpp"

#include "game_engine/math/Types.hpp"
#include "debug_tools/Console.hpp"

namespace math = game_engine::math;
namespace ge = game_engine;
namespace dt = debug_tools;

bool Wall::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/wall2.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    return ret == 0;
}

void Wall::Step(double delta_time) {

}

void Wall::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is wall speaking. Let's destroy ths object");

    Destroy();
}
