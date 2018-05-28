#include "Treasure.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

bool Treasure::Init(game_engine::OpenGLObject * object, game_engine::OpenGLTexture * texture) {
    
    int ret = WorldObject::Init(object, texture);

    SetCollision(1.0f, 1.0f);

    return ret == 0;
}

void Treasure::Step(double delta_time){
    
}
