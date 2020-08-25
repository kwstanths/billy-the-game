#include "World.hpp"

#include <fstream>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/utility/BasicFunctions.hpp"
#include "game_engine/graphics/AssimpHelp.hpp"
#include "debug_tools/Console.hpp"
#include "debug_tools/Timer.hpp"

#include "CollisionObject.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"
#include "StaticMap.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector(), TiledMap() {
    is_inited_ = false;
}

int World::Init(Input * input, std::string map_name, math::AABox<2> size, bool has_sun, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(30, 30, size.min_[0], size.max_[0], size.min_[1], size.max_[1], 200 * 200);
    if (ret) return ret;
    
    /* The map that this world represents */
    TiledMap::Init(map_name);

    has_sun_ = has_sun;

    /* Spawn static map, as prepared by TiledMap */
    for (size_t i = 0; i < packed_tiles_.size(); i++) {
        Vector3D pos = packed_tiles_[i].first;
        std::string name = packed_tiles_[i].second;
        NewObj<StaticMap>()->Init(pos.x(), pos.y(), pos.z(), name, this, engine);
    }

    /* Initialize a sun object */
    if (has_sun) {
        sun_ = NewObj<Sun>();
        sun_->Init(25.0f, 0.0f, 1000.0f, this, engine);
    } else {
        sun_ = nullptr;
    }

    /* Parse map layers, and spawn lights and physics objects, if needed */
    ReadMap(map_name + "_Tile Layer 1.csv", 0.02f, engine);
    ReadMap(map_name + "_Tile Layer 2.csv", 0.02f, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    TiledMap::Destroy();
    WorldSector::Destroy();

    is_inited_ = false;
    return true;
}

void World::ReadMap(std::string name, float z, game_engine::GameEngine * engine) {

    std::vector < std::vector < std::string>> map;
    TiledMap::ReadMap(name, map);

    MapProperties& map_properties = MapProperties::GetInstance();

    /* Set tile properties */
    int map_height = map.size();
    int map_width = map[0].size();
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            float x, y;
            GetXYFromTiled(i, j, x, y);
            if (map[i][j] == "-1") continue;
            
            int id = std::stoi(map[i][j]);
            float fire_intensity;
            if (map_properties.IsLight(id, fire_intensity)) {
                /* Spawn a point light, continous memory allocation for point lights is not set up */
                Fire * fire = new Fire();
                fire->Init(x, y, z, fire_intensity, this, engine, sun_);
            } else {
                /* Check if this tile has collision, spawn a collision object */
                std::string collision_string;
                bool has_collision = map_properties.HasCollision(std::stoi(map[i][j]), collision_string);
                if (has_collision) {
                    CollisionObject * co = new CollisionObject();
                    co->Init(x, y, z, collision_string, this, engine);
                }
            }
        }
    }

}
