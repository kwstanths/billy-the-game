#include "CollisionObject.hpp"

#include "debug_tools/Console.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool CollisionObject::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, std::string collision_string, ge::WorldSector * world, ge::GameEngine * engine) {
    
    int ret = PhysicsObject::Init(x, y, z);

    /* Parse the collision string, and set up the collision in the physics layer */
    std::vector<std::string> spl = ge::utility::split(collision_string, ",");
    Vector2D pos({ x - 0.5f, y - 0.5f});
    Vector2D min({ std::stof(spl[0]), std::stof(spl[1]) });
    Vector2D max({ std::stof(spl[2]), std::stof(spl[3]) });
    SetCollision(world->GetPhysicsEngine(), AABox<2>(pos + min, pos + max));

    return ret == 0;
}

