#include "Background.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool Background::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/background.obj", x, y, z);
    world->AddObject(this, x, y, z);

    /* Scale to cover the whole camera screen */
    Scale(25, 25, 25);

    return ret == 0;
}

void Background::Draw(game_engine::graphics::Renderer * renderer) {

    /* Simple drawing with no lightning */
    renderer->DrawSimple(this);
}

