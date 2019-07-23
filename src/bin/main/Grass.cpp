#include "Grass.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Grass::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, std::string name, ge::WorldSector * world, ge::GameEngine * engine) {
    
    tile_name = name;

    int ret = WorldObject::Init("assets/" + name + ".obj", x, y, z);
    world->AddObject(this, x, y, z);

    //Rotate(ge::math::GetRadians(-90.0f), glm::vec3(0, 0, 1));

    return ret == 0;
}

