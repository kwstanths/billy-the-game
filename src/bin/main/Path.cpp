#include "Path.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Path::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/path.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture, x, y, z);

    return ret == 0;
}

