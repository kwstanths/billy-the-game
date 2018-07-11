#include "Wall.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Wall::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/wall/wall.obj", x, y, z, true);

    SetCollision(1.0f, 1.0f);

    return ret == 0;
}

void Wall::Step(double delta_time) {

}

void Wall::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is wall speaking. Let's destroy ths object");

    Destroy();
}
