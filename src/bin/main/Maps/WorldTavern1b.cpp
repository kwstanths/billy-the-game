#include "WorldTavern1b.hpp"

#include "game_engine/math/AABox.hpp"

#include "Player.hpp"
#include "WorldPortal.hpp"

namespace math = game_engine::math;

WorldTavern1b::WorldTavern1b()
{
    is_inited_ = false;
}

int WorldTavern1b::Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * billy_world)
{
    int ret = World::Init(input, "tavern_1b", math::AABox<2>(math::Vector2D(-1.0f, -30.0f), math::Vector2D(30.0f, 1.0f)), false, camera, engine);

    /* Create the main player */
    player_ = NewObj<Player>();
    player_->Init(13, -7, 0.2f, input, camera, this, engine);

    {
        WorldPortal * world_portal = new WorldPortal();
        world_portal->Init(math::Vector3D(14, -7, 0.2f), this, billy_world, math::Vector3D(13, -7, 0.2f), engine, camera);
    }

    return 0;
}

int WorldTavern1b::Destroy()
{
    World::Destroy();

    return 0;
}
