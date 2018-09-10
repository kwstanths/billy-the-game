#include "Wall.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Wall::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/wall.obj", x, y, z);
    world->AddObject(this, x, y, z);

    return ret == 0;
}

void Wall::Draw(game_engine::graphics::Renderer * renderer) {

    renderer->DrawSimple(this);
}



