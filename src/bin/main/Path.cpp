#include "Path.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Path::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/path.obj", x, y, z);

    Scale(5, 5, 5);

    return ret == 0;
}

