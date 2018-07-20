#include "Lava.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Lava::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/lava.obj", x, y, z);

    SetObjectType(2);
    SetCollision(0.8f, 0.8f);

    return ret == 0;
}

