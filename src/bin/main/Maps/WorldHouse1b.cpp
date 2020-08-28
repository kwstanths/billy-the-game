#include "WorldHouse1b.hpp"

#include "game_engine/math/AABox.hpp"

#include "Player.hpp"
#include "WorldPortal.hpp"

namespace math = game_engine::math;

WorldHouse1b::WorldHouse1b()
{
    is_inited_ = false;
}

int WorldHouse1b::Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * first_floor)
{
    int ret = World::Init(input, "house1_b", math::AABox<2>(math::Vector2D(-1.0f, -30.0f), math::Vector2D(30.0f, 1.0f)), false, camera, engine);

    /* Create the main player */
    player_ = NewObj<Player>();
    player_->Init(13, -7, 0.2f, input, camera, this, engine);

    {
        WorldPortal * world_portal = new WorldPortal();
        world_portal->Init(math::Vector3D(10, -5, 0.2f), this, first_floor, math::Vector3D(9, -5, 0.2f), engine, camera);
    }

    return 0;
}

int WorldHouse1b::Destroy()
{
    World::Destroy();

    return 0;
}
