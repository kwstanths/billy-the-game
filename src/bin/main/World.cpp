#include "World.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/AssetManager.hpp"

#include "debug_tools/Console.hpp"

#include "Grass.hpp"
#include "Treasure.hpp"
#include "Player.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(ge::GameEngine * engine) {
    int ret = WorldSector::Init(500, 500, -500.0f, 500.0f, -500.0f, 500.0f, 500 * 500, engine);
    if (ret) return ret;

    /* Initialize the world */
    ge::AssetManager * asset_manager = engine->GetAssetManager();
    if (asset_manager == nullptr) return -1;
    
    /* Find oobjects */
    ge::OpenGLObject * object_tile = nullptr, * object_circle = nullptr;
    object_tile = asset_manager->FindObject("assets/tile.obj", &ret);
    if (ret != ge::Error::ERROR_NO_ERROR)  dt::ConsoleInfoL(dt::CRITICAL, "Asset not found", "name", "assets/tile.obj");
    object_circle = asset_manager->FindObject("assets/circle.obj", &ret);
    if (ret != ge::Error::ERROR_NO_ERROR) dt::ConsoleInfoL(dt::CRITICAL, "Asset not found", "name", "assets/circle.obj");

    /* Find textures */
    ge::OpenGLTexture * texture_grass = nullptr,
        *texture_treasure = nullptr,
        *texture_circle = nullptr,
        *texture_path = nullptr,
        *texture_wall = nullptr;
    texture_grass = asset_manager->FindTexture("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    texture_path = asset_manager->FindTexture("assets/path.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    texture_wall = asset_manager->FindTexture("assets/wall.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    texture_treasure = asset_manager->FindTexture("assets/treasure.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    texture_circle = asset_manager->FindTexture("assets/circle.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);


    /* Create grass */
    Grass * floor = NewObj<Grass>(0.0f, 0.0f, 0.0f);
    floor->Init(object_tile, texture_grass);
    floor->Scale(4, 4, 1);

    floor = NewObj<Grass>(4.0f, 0.0f, 0.0f);
    floor->Init(object_tile, texture_grass);
    floor->Scale(4, 4, 1);

    floor = NewObj<Grass>(0.0f, 4.0f, 0.0f);
    floor->Init(object_tile, texture_grass);
    floor->Scale(4, 4, 1);

    floor = NewObj<Grass>(4.0f, 4.0f, 0.0f);
    floor->Init(object_tile, texture_grass);
    floor->Scale(4, 4, 1);

    /* Create a path */
    floor = NewObj<Grass>(6.0f, 0.0f, 0.0f);
    floor->Init(object_tile, texture_path);
    floor->Scale(3, 4, 1);

    /* Create a wall */
    floor = NewObj<Grass>(0.0f, 6.5f, 0.0f);
    floor->Init(object_tile, texture_wall);
    floor->SetCollision(1.0f, 1.0f);

    floor = NewObj<Grass>(1.0f, 6.5f, 0.0f);
    floor->Init(object_tile, texture_wall);
    floor->SetCollision(1.0f, 1.0f);

    floor = NewObj<Grass>(2.0f, 6.5f, 0.0f);
    floor->Init(object_tile, texture_wall);
    floor->SetCollision(1.0f, 1.0f);

    floor = NewObj<Grass>(3.0f, 6.5f, 0.0f);
    floor->Init(object_tile, texture_wall);
    floor->SetCollision(1.0f, 1.0f);

    /* Create some collidable treasures */
    Treasure * tres = NewObj<Treasure>(0.0f, 1.5f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    
    tres = NewObj<Treasure>(-2.4f, 1.5f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    
    tres = NewObj<Treasure>(5.0f, 5.0f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    
    tres = NewObj<Treasure>(5.0f, -5.0f, 0.1f);
    tres->Init(object_tile, texture_treasure);

    /* Create a main player */
    Player * player = NewObj<Player>(0.0f, 0.0f, 0.2f);
    if (player == nullptr) return -1;
    player->Init(object_circle, texture_circle, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
