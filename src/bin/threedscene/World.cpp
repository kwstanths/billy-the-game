#include "World.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "debug_tools/Console.hpp"

#include "Player.hpp"
#include "Floor.hpp"
#include "Fire.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(10, 10, -10.0f, 10.0f, -10.0f, 10.0f, 10 * 10);
    if (ret) return ret;

    Player * player;
    
    //player = NewObj<Player>();
    //player->Init(0, 0, 0, input, camera, this, engine);

    for (int i = -6; i <= 6; i+=6){
        for (int j = -4; j <= 4; j+=4) {
            /* Create some players */
            Player * player = NewObj<Player>();
            player->Init(i, 0, j, input, camera, this, engine);
        }
    }

    /* Create floor */
    Floor * floor = NewObj<Floor>();
    floor->Init(0.0f, 0.0f, 0.0f, this);

    Fire * fire = NewObj<Fire>();
    fire->Init(0.0f, 5.0f, 0.0f, this, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}