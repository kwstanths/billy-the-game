#include "World.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "debug_tools/Console.hpp"

#include "Grass.hpp"
#include "Wall.hpp"
#include "Path.hpp"
#include "Treasure.hpp"
#include "Player.hpp"
#include "Sun.hpp"
#include "Fire.hpp"
#include "Cow.hpp"
#include "Lava.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(300, 300, -100.0f, 100.0f, -100.0f, 100.0f, 500 * 500);
    if (ret) return ret;

    /* Initialize the world */
    /* Create some grass */
    for (int x = -20; x < 20; x += 5)
        for (int y = -20; y < 20; y += 5) {
            NewObj<Grass>()->Init(x, y, 0.0f, this, engine);
        }
    
    /* Create a path*/
    for (int x = 0; x < 10; x += 5)
        for (int y = 15; y < 40; y += 5)
            NewObj<Path>()->Init(x, y, 0.0f, this, engine);
            
    /* Create a wall */
    for (int x = -15; x < 4; x += 1)
        NewObj<Wall>(true)->Init(x, 6.5f, 0.01f, this, engine);

    NewObj<Lava>()->Init(-5, 15, 0.0f, this, engine);

    /* Create some collidable treasures with some random ids */
    NewObj<Treasure>()->Init(0.0f, 1.5f, 0.01f, this, engine, 1);
    NewObj<Treasure>()->Init(-2.4f, 1.5f, 0.01f, this, engine, 2);
    NewObj<Treasure>()->Init(5.0f, -5.0f, 0.01f, this, engine, 3);
    NewObj<Treasure>()->Init(5.0f, 4.94f, 0.01f, this, engine, 4);
    NewObj<Treasure>()->Init(6.1f, 6.0f, 0.01f, this, engine, 5);
    /* Create and rotate some treasures */
    Treasure * tres = NewObj<Treasure>();
    tres->Init(7.0, 7.0f, 0.01f, this, engine, 6);
    tres->Rotate(math::GetRadians(15.0f), { 0,0,1 });
    /* Put something far up */
    tres = NewObj<Treasure>();
    tres->Init(0.0f, 30.0f, 0.01f, this, engine, 7);
    tres->Rotate(math::GetRadians(45.0f), { 0,0,1 });

    /* Adn then, there was light */
    Sun * sun = NewObj<Sun>();
    sun->Init(0.0f, 0.0f, 1000.0f, this, engine);

    /* Create some fire lights */
    NewObj<Fire>()->Init(-3.3f, -5.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(-3.3f, 0.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(-3.3f, 10.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(-3.3f, 15.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(3.3f, -5.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(3.3f, 0.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(3.3f, 10.8f, 1.0f, this, engine, sun);
    NewObj<Fire>()->Init(3.3f, 15.8f, 1.0f, this, engine, sun);

    /* Create a main player */
    Player * player = NewObj<Player>();
    player->Init(0.0f, 0.0f, 0.1f, input, camera, this, engine);

    /* Create a cow */
    NewObj<Cow>()->Init(2.0f, -2.0f, 0.1f, this, engine);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
