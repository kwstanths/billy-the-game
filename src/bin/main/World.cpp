#include "World.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

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
    /* Initialize assets */
    /* Objects */
    ge::OpenGLObject * object_tile = new ge::OpenGLObject();
    object_tile->Init("assets/tile.obj");
    if (ret) return ret;
    ge::OpenGLObject * object_circle = new ge::OpenGLObject();
    ret = object_circle->Init("assets/circle.obj");
    if (ret) return ret;

    /* Textures */
    ge::OpenGLTexture * texture_grass = new ge::OpenGLTexture();
    ret = texture_grass->Init("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_BMP);
    if (ret) return ret;
    ge::OpenGLTexture * texture_treasure = new ge::OpenGLTexture();
    ret = texture_treasure->Init("assets/treasure.bmp", ge::OpenGLTexture::TEXTURE_BMP);
    if (ret) return ret;
    ge::OpenGLTexture * texture_circle = new ge::OpenGLTexture();
    ret = texture_circle->Init("assets/circle.bmp", ge::OpenGLTexture::TEXTURE_BMP);
    if (ret) return ret;

    /* Create grass */
    NewObj<Grass>(-1.0f, 1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(0.0f, 1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(1.0f, 1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(-1.0f, 0.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(0.0f, 0.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(1.0f, 0.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(-1.0f, -1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(0.0f, -1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(1.0f, -1.0f, 0.0f, object_tile, texture_grass);
    NewObj<Grass>(-1.0f, 2.0f, 0.0f, object_tile, texture_grass);
 
    /* Create some collidable treasures */
    Treasure * tres = NewObj<Treasure>(0.0f, 1.5f, 0.1f, object_tile, texture_treasure);
    if (tres == nullptr) -1;
    tres->SetCollision(1.0f, 1.0f);
    
    tres = NewObj<Treasure>(-2.4f, 1.5f, 0.1f, object_tile, texture_treasure);
    if (tres == nullptr) -1;
    tres->SetCollision(1.0f, 1.0f);

    tres = NewObj<Treasure>(5.0f, 5.0f, 0.1f, object_tile, texture_treasure);
    if (tres == nullptr) return -1;
    tres->SetCollision(1.0f, 1.0f);

    /* Create a main player */
    Player * player = NewObj<Player>(0.0f, 0.0f, 0.2f, object_circle, texture_circle);
    if (player == nullptr) return -1;
    player->Init(engine);
    player->SetCollision(0.5f);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
