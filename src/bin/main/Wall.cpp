#include "Wall.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;

bool Wall::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/wall.obj", x, y, z, true);

    SetPosition(x, y, z);
    SetCollision(1.0f, 1.0f);

    return ret == 0;
}

void Wall::Step(double delta_time) {

}

void Wall::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is wall speaking. Let's destroy ths object");

    Destroy();
}
