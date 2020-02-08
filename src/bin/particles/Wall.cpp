#include "Wall.hpp"

namespace ge = game_engine;

Wall::Wall() {
    is_inited_ = false;
}

int Wall::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("assets/floor.obj", x, y, z);
    world->AddObject(this, x, y, z);

    is_inited_ = true;
    return ret == 0;
}
