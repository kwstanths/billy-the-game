#include "World.hpp"

#include <fstream>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/utility/BasicFunctions.hpp"
#include "game_engine/graphics/AssimpHelp.hpp"
#include "debug_tools/Console.hpp"
#include "debug_tools/Timer.hpp"

#include "Grass.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"
#include "StaticMap.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(30, 30, -200.0f, 200.0f, -200.0f, 200.0f, 200 * 200);
    if (ret) return ret;
    
    {
        dt::Timer timer;
        dt::CustomPrint(std::cout, "Reading map files... ");
        
        timer.Start();
        game_engine::graphics::GraphicsObject::InitObjectAtlas("assets/static_map.obj");
        timer.Stop();
        
        dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");
    }

    {
        dt::Timer timer;
        
        timer.Start();
        dt::CustomPrint(std::cout, "Reading map properties... ");
        map_properties_.ReadMap("roguelikeSheet_transparent.tsx");
        timer.Stop();

        dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");
    }

    {
        dt::Timer timer;
        timer.Start();
        dt::CustomPrint(std::cout, "Spawning map... ");
        
        /* Read correspondence tile region-position correspondence file */
        std::vector<std::vector<std::string>> tile_map;
        std::string line;
        std::ifstream myfile("packed_map_tiles.txt");
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                tile_map.push_back(ge::utility::split(line, " "));
            }
            myfile.close();
        }

        /* Spawn tile regions */
        for (size_t i = 0; i < tile_map.size(); i++) {
            ge::Real_t x = std::stof(tile_map[i][0]);
            ge::Real_t y = std::stof(tile_map[i][1]);
            std::string name = tile_map[i][2];
            NewObj<StaticMap>()->Init(x, y, 0, name, this, engine);
        }
        
        timer.Stop();
        dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");
    }

    sun_ = NewObj<Sun>();
    sun_->Init(25.0f, 0.0f, 1000.0f, this, engine);

    /* Create the main player */
    Player * player = NewObj<Player>();
    player->Init(25, 0, 0.2f, input, camera, this, engine);

    /* */
    ReadMap("billy_map_Tile Layer 1.csv", 0.02f, engine);
    ReadMap("billy_map_Tile Layer 2.csv", 0.02f, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}


/* Not used */
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
            if (map_[i][j] == "416") {
                /* Spawn a point light, continous memory allocation for point lights is not set up */
                Fire * fire = new Fire();
                fire->Init(j - map_width_2, map_height_2 - i, z, this, engine, sun_);
            } else {
                //NewObj<Grass>()->Init(j - map_width_2, map_height_2 - i, z, "roguelikeSheet_transparent_" + map_[i][j], this, engine, map_properties_);
            }

        }
    }

    timer.Stop();
    dt::CustomPrint(std::cout, "DONE, " + timer.ToString() + "\n");

}
