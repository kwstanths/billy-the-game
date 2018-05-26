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
    ge::OpenGLTexture * texture_grass = nullptr, * texture_treasure = nullptr, * texture_circle = nullptr;
    texture_grass = asset_manager->FindTexture("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    if (ret != ge::Error::ERROR_NO_ERROR) dt::ConsoleInfoL(dt::CRITICAL, "Asset not found", "name", "assets/grass.bmp");
    texture_treasure = asset_manager->FindTexture("assets/treasure.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    if (ret != ge::Error::ERROR_NO_ERROR) dt::ConsoleInfoL(dt::CRITICAL, "Asset not found", "name", "assets/treasure.bmp");
    texture_circle = asset_manager->FindTexture("assets/circle.bmp", ge::OpenGLTexture::TEXTURE_BMP, &ret);
    if (ret != ge::Error::ERROR_NO_ERROR) dt::ConsoleInfoL(dt::CRITICAL, "Asset not found", "name", "assets/circle.bmp");

    /* Create grass */
    /* We should probably check the return value of NewObj */
    NewObj<Grass>(-1.0f, 1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(0.0f, 1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(1.0f, 1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(-1.0f, 0.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(0.0f, 0.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(1.0f, 0.0f, 0.0)->Init(object_tile, texture_grass);
    NewObj<Grass>(-1.0f, -1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(0.0f, -1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(1.0f, -1.0f, 0.0f)->Init(object_tile, texture_grass);
    NewObj<Grass>(-1.0f, 2.0f, 0.0f)->Init(object_tile, texture_grass);
 
    /* Create some collidable treasures */
    Treasure * tres = NewObj<Treasure>(0.0f, 1.5f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    if (tres == nullptr) -1;
    tres->SetCollision(1.0f, 1.0f);
    
    tres = NewObj<Treasure>(-2.4f, 1.5f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    if (tres == nullptr) -1;
    tres->SetCollision(1.0f, 1.0f);

    tres = NewObj<Treasure>(5.0f, 5.0f, 0.1f);
    tres->Init(object_tile, texture_treasure);
    if (tres == nullptr) return -1;
    tres->SetCollision(1.0f, 1.0f);

    /* Create a main player */
    Player * player = NewObj<Player>(0.0f, 0.0f, 0.2f);
    if (player == nullptr) return -1;
    player->Init(object_circle, texture_circle, engine);
    player->SetCollision(0.5f);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
