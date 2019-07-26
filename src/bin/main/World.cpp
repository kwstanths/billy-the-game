#include "World.hpp"

#include <fstream>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/utility/BasicFunctions.hpp"
#include "debug_tools/Console.hpp"
#include "debug_tools/Timer.hpp"

#include "Grass.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(300, 300, -200.0f, 200.0f, -200.0f, 200.0f, 500 * 500);
    if (ret) return ret;


    /* And then, there was light */
    sun_ = NewObj<Sun>();
    sun_->Init(25.0f, 0.0f, 1000.0f, this, engine);

    map_properties_.ReadMap("roguelikeSheet_transparent.tsx");
    //NewObj<Grass>()->Init(26.0f, 0.0f, 0.0f, "roguelikeSheet_transparent_61", this, engine, map_properties_);
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
    player->Init(25.0f, 0.0f, 0.2f, input, camera, this, engine);


    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}

void World::ReadMap(std::string name, float z, game_engine::GameEngine * engine) {

    dt::Timer timer;
    timer.Start();
    dt::CustomPrint(std::cout, "Reading map " + name + " ... ");

    std::vector<std::vector<std::string>> map_;

    std::string line;
    std::ifstream myfile(name);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            map_.push_back(ge::utility::split(line, ","));
        }
        myfile.close();
    }

    timer.Stop();
    dt::CustomPrint(std::cout, "DONE, " + timer.ToString() +"\n");
    
    timer.Start();
    dt::CustomPrint(std::cout, "Spawing map... ");

    int map_height = 154;
    int map_width = 178;
    float map_width_2 = ((float)map_width) / 2.0f;
    float map_height_2 = ((float) map_height) / 2.0f;
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (map_[i][j] == "-1") continue;
            if (map_[i][j] == "416")
                NewObj<Fire>()->Init(j - map_width_2, map_height_2 - i, z, "roguelikeSheet_transparent_" + map_[i][j], this, engine, sun_);
            else
                NewObj<Grass>()->Init(j - map_width_2, map_height_2 - i, z, "roguelikeSheet_transparent_" + map_[i][j], this, engine, map_properties_);

        }
    }

    timer.Stop();
    dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");

}
