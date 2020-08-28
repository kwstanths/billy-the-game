#include "WorldBillyMap.hpp"

#include "game_engine/math/AABox.hpp"

#include "Player.hpp"
#include "WorldPortal.hpp"

namespace math = game_engine::math;

WorldBillyMap::WorldBillyMap()
{
    is_inited_ = false;
}

int WorldBillyMap::Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * tavern1_world, World * house1_world)
{
    int ret = World::Init(input, "billy_map", math::AABox<2>(math::Vector2D(-1.0f, -200.0f), math::Vector2D(200.0f, 1.0f)), true, camera, engine);

    /* Create the main player */
    player_ = NewObj<Player>();
    player_->Init(106, -77, 0.2f, input, camera, this, engine);

    WorldPortal * tavern1_portal = new WorldPortal();
    tavern1_portal->Init(math::Vector3D(117, -73, 0.2f), this, tavern1_world, math::Vector3D(6, -18, 0.2f), engine, camera);

    WorldPortal * house1_portal = new WorldPortal();
    house1_portal->Init(math::Vector3D(90, -78, 0.2f), this, house1_world, math::Vector3D(5, -8, 0.2f), engine, camera);

    return 0;
}

int WorldBillyMap::Destroy()
{
    World::Destroy();

    return 0;
}
