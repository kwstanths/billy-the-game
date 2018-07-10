#include "Grass.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Grass::Init(float x, float y, float z, game_engine::GameEngine * engine) {
    
    int ret = WorldObject::Init("assets/grass/grass.obj", x, y, z);
    
    Scale(5, 5, 5);

    return ret == 0;
}

