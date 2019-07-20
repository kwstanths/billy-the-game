#include "World.hpp"

#include <fstream>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/utility/BasicFunctions.hpp"
#include "debug_tools/Console.hpp"

#include "Grass.hpp"
#include "Wall.hpp"
#include "Path.hpp"
#include "Treasure.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"
#include "Cow.hpp"
#include "Lava.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(300, 300, -100.0f, 100.0f, -100.0f, 100.0f, 500 * 500);
    if (ret) return ret;

    //NewObj<Grass>()->Init(0.0f, 0.0f, 0.0f, "rogulikerpg_11.obj", this, engine);

    /* And then, there was light */
    sun_ = NewObj<Sun>();
    sun_->Init(0.0f, 0.0f, 1000.0f, this, engine);

    ReadMap("billy_map_Tile Layer 1.csv", 0.0f, engine);
    ReadMap("billy_map_Tile Layer 2.csv", 0.1f, engine);


    /* Create some fire lights */
    //NewObj<Fire>()->Init(0.0f, 0.0f, 1.0f, this, engine, sun);
    
    /* Create some fire along the path */
    //for (int x = -10; x < 10; x += 8)
    //    for (int y = -10; y < 10; y += 8)
    //        NewObj<Fire>()->Init(x, y, 1.0f, this, engine, sun);


    /* Create the main player */
    Player * player = NewObj<Player>();
    player->Init(0.0f, 0.0f, 0.4f, input, camera, this, engine);


    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}

void World::ReadMap(std::string name, float z, game_engine::GameEngine * engine) {

    std::vector<std::vector<std::string>> map_;

    std::string line;
    std::ifstream myfile(name);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            map_.push_back(ge::utility::split(line, ","));
        }
        myfile.close();
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (map_[i][j] == "-1") continue;
            if (map_[i][j] == "416")
                NewObj<Fire>()->Init(j - 50, 50 - i, z, "rogulikerpg_" + map_[i][j] + ".obj", this, engine, sun_);
            else
                NewObj<Grass>()->Init(j - 50, 50 -i, z, "rogulikerpg_" + map_[i][j] + ".obj", this, engine);

        }
    }

}
