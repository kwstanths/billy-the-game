#include "World.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/AssetManager.hpp"

#include "debug_tools/Console.hpp"

#include "Grass.hpp"
#include "Wall.hpp"
#include "Path.hpp"
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
    /* Create grass */
    Grass * floor = NewObj<Grass>(0.0f, 0.0f, 0.0f);
    floor->Init(engine);
    floor = NewObj<Grass>(5.0f, 0.0f, 0.0f);
    floor->Init(engine);
    floor = NewObj<Grass>(0.0f, 5.0f, 0.0f);
    floor->Init(engine);
    floor = NewObj<Grass>(5.0f, 5.0f, 0.0f);
    floor->Init(engine);

    /* Create a path */
    Path * path = NewObj<Path>(9.0f, 0.0f, 0.0f);
    path->Init(engine);
    path->Scale(3, 4, 1);
    path = NewObj<Path>(9.0f, 4.0f, 0.0f);
    path->Init(engine);
    path->Scale(3, 4, 1);

    /* Create a wall */
    Wall * wall = NewObj<Wall>(-1.0f, 6.5f, 0.1f);
    wall->Init(engine);
    wall = NewObj<Wall>(0.0f, 6.5f, 0.1f);
    wall->Init(engine);
    wall = NewObj<Wall>(1.0f, 6.5f, 0.1f);
    wall->Init(engine);
    wall = NewObj<Wall>(2.0f, 6.5f, 0.1f);
    wall->Init(engine);
    wall = NewObj<Wall>(3.0f, 6.5f, 0.1f);
    wall->Init(engine);
    wall = NewObj<Wall>(4.0f, 6.5f, 0.1f);
    wall->Init(engine);

    /* Create some collidable treasures */
    Treasure * tres = NewObj<Treasure>(0.0f, 1.5f, 0.1f);
    tres->Init(engine);
    tres = NewObj<Treasure>(-2.4f, 1.5f, 0.1f);
    tres->Init(engine);
    tres = NewObj<Treasure>(5.0f, 5.0f, 0.1f);
    tres->Init(engine);
    tres = NewObj<Treasure>(5.0f, -5.0f, 0.1f);
    tres->Init(engine);

    /* Create a main player */
    Player * player = NewObj<Player>(0.0f, 0.0f, 0.2f);
    player->Init(engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
