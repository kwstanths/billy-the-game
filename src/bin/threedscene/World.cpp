#include "World.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "debug_tools/Console.hpp"

#include "Player.hpp"
#include "Floor.hpp"
#include "FloorNormals.hpp"
#include "Fire.hpp"
#include "Sun.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(10, 10, -80.0f, 80.0f, -80.0f, 80.0f, 10 * 10);
    if (ret) return ret;

    /* Create a tree */
    //{
    //    Player * player;
    //    player = NewObj<Player>();
    //    player->Init(0, 9.3, 0, input, camera, this, engine);
    //}
    
    //for (int i = -6; i <= 6; i+=6){
    //    for (int j = -4; j <= 4; j+=4) {
    //        /* Create some players */
    //        Player * player = NewObj<Player>();
    //        player->Init(1.0f * i, 0, 1.0f * j, input, camera, this, engine);
    //    } 
    //}

    /* Some lights */
    //for (int i = -60; i <= 60; i+=25){
    //    for (int j = -60; j <= 60; j+=20) {
    //        /* Create some players */
    //        Fire * fire = new Fire();
    //        fire->Init(1.0f * i, 10, 1.0f * j, this, engine);
    //    }
    //}

    /* The displacement object */
    Floor * floor = NewObj<Floor>();
    floor->Init(0.0f, 0.0f, 0.0f, this);

    FloorNormals * floor_normals = NewObj<FloorNormals>();
    floor_normals->Init(0.0f, 0.0f, 0.0f, this);

    /* A sun */
    Sun * sun = NewObj<Sun>();
    sun->Init(0.0f, 12.0f, 0.0f, this, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
