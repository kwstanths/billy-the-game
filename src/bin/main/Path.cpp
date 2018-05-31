#include "Path.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Path::Init(game_engine::GameEngine * engine) {

    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/path.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture);

    return ret == 0;
}

void Path::Step(double delta_time) {

}
