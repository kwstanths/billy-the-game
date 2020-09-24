#include "World.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "debug_tools/Console.hpp"

#include "Player.hpp"
#include "Heightmap.hpp"
#include "Floor.hpp"
#include "Fire.hpp"
#include "Sun.hpp"
#include "Water.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(10, 10, -550.0f, 550.0f, -550.0f, 550.0f, 10 * 10);
    if (ret) return ret;

    /* Create a tree */
    //{
    //    Player * player;
    //    player = NewObj<Player>();
    //    player->Init(0, 7.9, 8, input, camera, this, engine);
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

    Floor * floor = NewObj<Floor>();
    floor->Init(150, 1, 150, this);

    /* Draw some displacement maps */
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(0.0f, 0.0f, 0.0f, this, "textures/DoubleBasin_big.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(0.0f, 0.0f, 100.0f, this, "textures/Heightmap_Mountain.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(000.0f, 0.0f, 200.0f, this, "textures/world.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(100.0f, 0.0f, 0.0f, this, "textures/Heightmap_Billow.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(100.0f, 0.0f, 100.0f, this, "textures/Heightmap_Plateau.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(100.0f, 0.0f, 200.0f, this, "textures/Heightmap_Plateau.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(200.0f, 0.0f, 0.0f, this, "textures/DoubleBasin_big.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(200.0f, 0.0f, 100.0f, this, "textures/Heightmap_Mountain.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(300.0f, 0.0f, 0.0f, this, "textures/Heightmap_Billow.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(300.0f, 0.0f, 100.0f, this, "textures/Heightmap_Plateau.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(300.0f, 0.0f, 200.0f, this, "textures/Heightmap_Plateau.png");
    }
    {
        Heightmap * heightmap = NewObj<Heightmap>();
        heightmap->Init(200.0f, 0.0f, 200.0f, this, "textures/world.png");
    }

    /* Draw some water patches */
    {
        Water * water = NewObj<Water>();
        water->Init(0.0f, 6.0f, 0.0f, this);
    }
    {
        Water * water = NewObj<Water>();
        water->Init(0.0f, 6.0f, 320.0f, this);
    }
    {
        Water * water = NewObj<Water>();
        water->Init(320.0f, 6.0f, 00.0f, this);
    }
    {
        Water * water = NewObj<Water>();
        water->Init(320.0f, 6.0f, 320.0f, this);
    }

    /* Draw a sun */
    Sun * sun = NewObj<Sun>();
    sun->Init(0.0f, 12.0f, 0.0f, this, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
