#include "Path.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Path::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret = WorldObject::Init("assets/path/path.obj", x, y, z);

    Scale(5, 5, 0);

    return ret == 0;
}

