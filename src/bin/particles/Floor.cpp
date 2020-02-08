#include "Floor.hpp"

namespace ge = game_engine;

Floor::Floor() {
    is_inited_ = false;
}

int Floor::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init();
    ret = GraphicsObject::Init(x, y, z, "assets/floor.obj");
    world->AddObject(this, x, y, z);
    /* Initialize the graphics layer */

    Rotate(ge::math::GetRadians(90), { -1,0,0 });
    Scale(40, 40, 40);

    is_inited_ = true;
    return ret == 0;
}
