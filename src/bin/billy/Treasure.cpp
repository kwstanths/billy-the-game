#include "Treasure.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

bool Treasure::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine, size_t id) {
    
    int ret = WorldObject::Init("assets/treasure.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    SetCollision(math::Rectangle2D(x, y, 1.0f, 1.0f));

    id_ = id;

    return ret == 0;
}

void Treasure::Step(double delta_time){

}

void Treasure::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is treasure " + std::to_string(id_) + " speaking");
}
