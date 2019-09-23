#include "Player.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace math = game_engine::math;

Player::Player(): WorldObject() {
    engine_ = nullptr;

    is_inited_ = false;
}

int Player::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, Input * input, Camera * camera, ge::WorldSector * world, ge::GameEngine * engine) {

    input_ = input;
    engine_ = engine;
    camera_ = camera;

    int ret = WorldObject::Init("assets/Tree.obj", x, y, z);
    world->AddObject(this, x, y, z);

    /*Rotate(ge::math::GetRadians(90.0f), glm::vec3(0, 1, 0));
    Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));
*/
    is_inited_ = true;
    return ret == 0;
}

int Player::Destroy() {

    /* TODO */

    return 0;
}


