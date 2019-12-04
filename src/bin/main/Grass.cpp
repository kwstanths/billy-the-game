#include "Grass.hpp"

#include "debug_tools/Console.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Grass::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, std::string name, ge::WorldSector * world, ge::GameEngine * engine, MapProperties& map_properties) {
    
    tile_name = name;

    int ret = WorldObject::Init("assets/" + name + ".obj", x, y, z);
    world->AddObject(this, x, y, z);

    std::string collision_string;
    if (map_properties.HasCollision(name, collision_string)) {
        std::vector<std::string> spl = ge::utility::split(collision_string, ",");
        Vector2D pos({ x - 0.5f, y - 0.5f});
        Vector2D min({ std::stof(spl[0]), std::stof(spl[1]) });
        Vector2D max({ std::stof(spl[2]), std::stof(spl[3]) });
        SetCollision(world->GetPhysicsEngine(), AABox<2>(pos + min, pos + max));
    }

    return ret == 0;
}

