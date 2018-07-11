#include "World.hpp"

#include "debug_tools/Console.hpp"

#include "Grass.hpp"
#include "Wall.hpp"
#include "Path.hpp"
#include "Treasure.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(300, 300, -500.0f, 500.0f, -500.0f, 500.0f, 500 * 500);
    if (ret) return ret;

    /* Initialize the world */
    /* Create some grass */
    for (int width = -15; width < 15; width += 5)
        for (int height = -15; height < 15; height += 5) {
            NewObj<Grass>()->Init(width, height, 0.0f, engine);
    }
    
    /* Create a path*/
    NewObj<Path>()->Init(0.0f, 15.0f, 0.01f, engine);
    NewObj<Path>()->Init(5.0f, 15.0f, 0.01f, engine);
    NewObj<Path>()->Init(0.0f, 20.0f, 0.01f, engine);
    NewObj<Path>()->Init(5.0f, 20.0f, 0.01f, engine);

    /* Create a wall */
    NewObj<Wall>(true)->Init(-7.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-6.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-5.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-4.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-3.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-2.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(-1.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(0.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(1.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(2.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(3.0f, 6.5f, 0.01f, engine);
    NewObj<Wall>(true)->Init(4.0f, 6.5f, 0.01f, engine);

    /* Create some collidable treasures with some random ids */
    NewObj<Treasure>()->Init(0.0f, 1.5f, 0.01f, engine, 1);
    NewObj<Treasure>()->Init(-2.4f, 1.5f, 0.01f, engine, 2);
    NewObj<Treasure>()->Init(5.0f, -5.0f, 0.01f, engine, 3);
    NewObj<Treasure>()->Init(5.0f, 4.94f, 0.01f, engine, 4);
    NewObj<Treasure>()->Init(6.1f, 6.0f, 0.01f, engine, 5);
    /* Create and rotate some treasures */
    Treasure * tres = NewObj<Treasure>();
    tres->Init(7.0, 7.0f, 0.01f, engine, 6);
    tres->Rotate(ge::GetRadians(15.0f), 2);
    /* Put something far up */
    tres = NewObj<Treasure>();
    tres->Init(0.0f, 30.0f, 0.01f, engine, 7);
    tres->Rotate(ge::GetRadians(45.0f), 2);

    /* Adn then, there was light */
    Sun * sun = NewObj<Sun>();
    sun->Init(0.0f, 0.0f, 1000.0f, engine);

    /* Create some fire lighfts */
    NewObj<Fire>()->Init(3.3f, -5.8f, 1.0f, engine, sun);
    NewObj<Fire>()->Init(3.3f, 0.8f, 1.0f, engine, sun);
    NewObj<Fire>()->Init(3.3f, 10.8f, 1.0f, engine, sun);
    NewObj<Fire>()->Init(3.3f, 15.8f, 1.0f, engine, sun);

    /* Create a main player */
    Player * player = NewObj<Player>();
    player->Init(0.0f, 0.0f, 0.1f, input, camera, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
