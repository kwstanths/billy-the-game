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
    int ret = WorldSector::Init(500, 500, -500.0f, 500.0f, -500.0f, 500.0f, 500 * 500);
    if (ret) return ret;

    /* Initialize the world */
    /* Create some grass */
    Grass * floor = NewObj<Grass>();
    floor->Init(0.0f, 0.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(5.0f, 0.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(0.0f, 5.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(5.0f, 5.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(-5.0f, 0.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(0.0f, -5.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(-5.0f, -5.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(5.0f, -5.0f, 0.0f, engine);
    floor = NewObj<Grass>();
    floor->Init(-5.0f, 5.0f, 0.0f, engine);

    /* Create a path */
    Path * path = NewObj<Path>();
    path->Init(9.5f, 0.0f, 0.0f, engine);
    path->Scale(4, 4, 1);
    path = NewObj<Path>();
    path->Init(9.5f, 4.0f, 0.0f, engine);
    path->Scale(4, 4, 1);

    /* Create a wall */
    Wall * wall = NewObj<Wall>(true);
    wall->Init(-1.0f, 6.5f, 0.1f, engine);
    wall = NewObj<Wall>(true);
    wall->Init(0.0f, 6.5f, 0.1f, engine);
    wall = NewObj<Wall>(true);
    wall->Init(1.0f, 6.5f, 0.1f, engine);
    wall = NewObj<Wall>(true);
    wall->Init(2.0f, 6.5f, 0.1f, engine);
    wall = NewObj<Wall>(true);
    wall->Init(3.0f, 6.5f, 0.1f, engine);
    wall = NewObj<Wall>(true);
    wall->Init(4.0f, 6.5f, 0.1f, engine);

    /* Create some collidable treasures with some random ids */
    Treasure * tres = NewObj<Treasure>();
    tres->Init(0.0f, 1.5f, 0.1f, engine, 1);
    tres = NewObj<Treasure>();
    tres->Init(-2.4f, 1.5f, 0.1f, engine, 2);
    tres = NewObj<Treasure>();
    tres->Init(5.0f, -5.0f, 0.1f, engine, 3);
    tres = NewObj<Treasure>();
    tres->Init(5.0f, 4.94f, 0.1f, engine, 4);
    tres = NewObj<Treasure>();
    tres->Init(6.1f, 6.0f, 0.1f, engine, 5);
    tres = NewObj<Treasure>();
    tres->Init(7.0, 7.0f, 0.1f, engine, 6);
    tres->Rotate(ge::GetRadians(15.0f), 2);
    tres = NewObj<Treasure>();
    tres->Init(0.0f, 30.0f, 0.1f, engine, 7); /* Put something far up */
    tres->Rotate(ge::GetRadians(45.0f), 2);

    /* Create a main player */
    Player * player = NewObj<Player>();
    player->Init(0.0f, 0.0f, 0.2f, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
