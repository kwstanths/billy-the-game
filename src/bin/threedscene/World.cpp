#include "World.hpp"

#include <vector>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/graphics/Material.hpp"
#include "game_engine/core/FileSystem.hpp"

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

    ///* Create a tree */
    //{
    //    Player * player;
    //    player = NewObj<Player>();
    //    player->Init(0, 7.9, 8, input, camera, this, engine);
    //}
    
    for (int i = -50; i <= 50; i+=10){
        for (int j = -50; j <= 50; j+=10) {
            /* Create some players */
            Player * player = NewObj<Player>();
            player->Init(10 + 1.0f * i, 0, 1.0f * j, input, camera, this, engine);
        } 
    }

    /* Some lights */
    //for (int i = -60; i <= 60; i+=25){
    //    for (int j = -60; j <= 60; j+=20) {
    //        /* Create some players */
    //        Fire * fire = new Fire();
    //        fire->Init(1.0f * i, 10, 1.0f * j, this, engine);
    //    }
    //}

    /* Create a floor, solid green color everywhere */
    Floor * floor = NewObj<Floor>();
    floor->Init(150, -0.8, 150, this);

    /* Draw some islands */
    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(300.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, 300.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(300.0f, -5.0f, 300.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(-300.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, -300.0f, this, "textures/Heightmap_Island.png");
    //}

    /* Draw some water patches */
    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0, 0.0f, 480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, 480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, 480.0f, this);
    //}

    /* Draw a sun */
    Sun * sun = NewObj<Sun>();
    sun->Init(300, 300, 0.0f, this, engine);

    ///* Create a skybox */
    //std::string asssets_directory = ge::FileSystem::GetInstance().GetDirectoryAssets();
    //std::vector<std::string> faces
    //{
    //    asssets_directory + "textures/ulukai/right.png",
    //    asssets_directory + "textures/ulukai/left.png",
    //    asssets_directory + "textures/ulukai/top.png",
    //    asssets_directory + "textures/ulukai/bottom.png",
    //    asssets_directory + "textures/ulukai/front.png",
    //    asssets_directory + "textures/ulukai/back.png"
    //};
    //ge::graphics::MaterialSkybox * skybox = new ge::graphics::MaterialSkybox(faces);
    //engine->GetRenderer()->SetSkybox(skybox);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
