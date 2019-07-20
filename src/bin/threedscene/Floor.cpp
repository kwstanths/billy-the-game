#include "Floor.hpp"

namespace ge = game_engine;

Floor::Floor() {
    is_inited_ = false;
}

int Floor::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("assets/floor.obj", x, y, z);
    world->AddObject(this, x, y, z, true);

    Scale(50, 50, 50);
    Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));

    is_inited_ = true;
    return ret == 0;
}
