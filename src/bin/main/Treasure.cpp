#include "Treasure.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

bool Treasure::Init(ge::GameEngine * engine) {
    
    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/treasure.bmp",ge::OpenGLTexture::TEXTURE_BMP, &ret);

    ret = WorldObject::Init(object, texture);

    SetCollision(1.0f, 1.0f);

    return ret == 0;
}

void Treasure::Step(double delta_time){
    
}
