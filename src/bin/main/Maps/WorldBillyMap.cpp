#include "WorldBillyMap.hpp"

#include "game_engine/math/AABox.hpp"

#include "Player.hpp"
#include "WorldPortal.hpp"

namespace math = game_engine::math;

WorldBillyMap::WorldBillyMap()
{
    is_inited_ = false;
}

int WorldBillyMap::Init(Input * input, Camera * camera, game_engine::GameEngine * engine, WorldSector * test_world)
{
    int ret = World::Init(input, "billy_map", math::AABox<2>(math::Vector2D(-1.0f, -200.0f), math::Vector2D(200.0f, 1.0f)), true, camera, engine);

    /* Create the main player */
    Player * player = NewObj<Player>();
    player->Init(106, -77, 0.2f, input, camera, this, engine);

    WorldPortal * world_portal = new WorldPortal();
    world_portal->Init(math::Vector3D(117, -73, 0.2f), this, test_world, math::Vector3D(6, -18, 0.2f), engine, camera);

    return 0;
}

int WorldBillyMap::Destroy()
{
    World::Destroy();

    return 0;
}
