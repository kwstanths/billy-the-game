#include "Cow.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/Debugger.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace math = game_engine::math;
namespace dt = debug_tools;

bool Cow::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    /* This is just playing here, proceed with caution */

    int ret = WorldObject::Init("assets/teapot.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    Scale(0.005f, 0.005f, 0.005f);
    //SetCollision(math::Circle2D(x, y + 0.5f, ge::Real_t(0.5)));    

    return ret == 0;
}

void Cow::Step(double delta_time) {

}

void Cow::Draw(game_engine::graphics::Renderer * renderer) {
    renderer->DrawSimple(this);
}

void Cow::Interact() {
    dt::Console(dt::INFO, "MOOOOOOOOOOOOOH");
}
