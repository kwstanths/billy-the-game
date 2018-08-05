#include "Lava.hpp"

#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;


bool Lava::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/lava.obj", x, y, z);

    SetObjectType(2);
    SetCollision(math::Rectangle2D(x, y, 0.8f, 0.8f));

    return ret == 0;
}

