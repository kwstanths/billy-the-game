#include "Wall.hpp"

#include "game_engine/math/Types.hpp"
#include "debug_tools/Console.hpp"

namespace math = game_engine::math;
namespace ge = game_engine;
namespace dt = debug_tools;

bool Wall::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/wall.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    SetCollision(math::Rectangle2D(x, y, 1.0f, 1.0f));

    return ret == 0;
}

void Wall::Step(double delta_time) {
    
    if (delete_object_) WorldObject::Destroy();
}

void Wall::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is wall speaking. Let's destroy ths object");

    /* The object will be deleted in the next frame */
    delete_object_ = true;
}
