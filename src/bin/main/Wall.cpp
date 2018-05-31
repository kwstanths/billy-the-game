#include "Wall.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Wall::Init(game_engine::GameEngine * engine) {

    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/wall.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture);

    SetCollision(1.0f, 1.0f);

    return ret == 0;
}

void Wall::Step(double delta_time) {

}
