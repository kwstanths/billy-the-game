#include "Cow.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/Debugger.hpp"

#include "debug_tools/Console.hpp"

namespace ge = game_engine;
namespace math = game_engine::math;
namespace dt = debug_tools;

bool Cow::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/cow.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    Scale(0.2f, 0.2f, 0.2f);

    return ret == 0;
}

void Cow::Step(double delta_time) {

}

void Cow::Interact() {
    dt::Console(dt::INFO, "MOOOOOOOOOOOOOH");
}
