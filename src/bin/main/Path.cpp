#include "Path.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Path::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/path.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(5, 5, 5);

    return ret == 0;
}

