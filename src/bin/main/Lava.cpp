#include "Lava.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Lava::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/lava.obj", x, y, z);

    SetObjectType(2);
    SetCollision(0.8f, 0.8f);

    return ret == 0;
}

