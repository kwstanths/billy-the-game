#include "Grass.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Grass::Init(game_engine::GameEngine * engine) {
    
    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture);
    
    Scale(5, 5, 1);

    return ret == 0;
}

void Grass::Step(double delta_time) {
    
}
