#include "Cow.hpp"

#include "game_engine/math/HelpFunctions.hpp"

#include "debug_tools/Console.hpp"
namespace ge = game_engine;
namespace dt = debug_tools;

bool Cow::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::GameEngine * engine) {

    int ret = WorldObject::Init("assets/cow.obj", x, y, z, true);
    SetPosition(x, y, 0.1f);
    Scale(0.2f, 0.2f, 0.2f);
    Rotate(ge::math::GetRadians(90.0f), 0);

    SetCollision(0.5f, 2.0f);

    return ret == 0;
}

void Cow::Step(double delta_time) {

}

void Cow::Interact() {
    dt::Console(dt::INFO, "MOOOOOOOOOOOOOH");
}
