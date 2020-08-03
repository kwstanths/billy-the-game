#include "StaticMap.hpp"    

#include "debug_tools/Console.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool StaticMap::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, std::string name, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/" + name + ".obj", x, y, z);
    world->AddObject(this, x, y, z);
    
    return ret == 0;
}

