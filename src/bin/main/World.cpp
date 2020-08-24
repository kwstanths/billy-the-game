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

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    /* Initialize a world of size (-1, -200) to (200, 1), divided into a 30x30 grid, that can hold up to 200*200 world objects */
    int ret = WorldSector::Init(30, 30, -1, 200, -200, 1, 200 * 200);
    if (ret) return ret;
    
    /* The map that this world represents */
    std::string map_name = "billy_map";
    TiledMap::Init(map_name);

    /* Parse tile properties */
    {
        dt::Timer timer;

        timer.Start();
        dt::CustomPrint(std::cout, "Reading map properties... ");
        map_properties_.ReadMap("roguelikeSheet_transparent.tsx");
        timer.Stop();

        dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");
    }

    /* Spawn static map, as prepared by TiledMap */
    for (size_t i = 0; i < packed_tiles_.size(); i++) {
        Vector3D pos = packed_tiles_[i].first;
        std::string name = packed_tiles_[i].second;
        NewObj<StaticMap>()->Init(pos.x(), pos.y(), pos.z(), name, this, engine);
    }

    /* Initialize a sun object */
    sun_ = NewObj<Sun>();
    sun_->Init(25.0f, 0.0f, 1000.0f, this, engine);

    /* Create the main player */
    Player * player = NewObj<Player>();
    //player->Init(6, -18, 0.2f, input, camera, this, engine);
    player->Init(106, -77, 0.2f, input, camera, this, engine);

    /* Parse map layers, and spawn lights and physics objects, if needed */
    ReadMap(map_name + "_Tile Layer 1.csv", 0.02f, engine);
    ReadMap(map_name + "_Tile Layer 2.csv", 0.02f, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}

void World::ReadMap(std::string name, float z, game_engine::GameEngine * engine) {

    std::vector < std::vector < std::string>> map;
    TiledMap::ReadMap(name, map);

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
            if (map_properties_.IsLight(id, fire_intensity)) {
                /* Spawn a point light, continous memory allocation for point lights is not set up */
                Fire * fire = new Fire();
                fire->Init(x, y, z, fire_intensity, this, engine, sun_);
            } else {
                /* Check if this tile has collision, spawn a collision object */
                std::string collision_string;
                bool has_collision = map_properties_.HasCollision(std::stoi(map[i][j]), collision_string);
                if (has_collision) {
                    CollisionObject * co = new CollisionObject();
                    co->Init(x, y, z, collision_string, this, engine);
                }
            }
        }
    }

}
