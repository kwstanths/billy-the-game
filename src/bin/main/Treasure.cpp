#include "Treasure.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

bool Treasure::Init(game_engine::OpenGLObject * object, game_engine::OpenGLTexture * texture) {
    
    int ret = WorldObject::Init(object, texture);

    return ret == 0;
}

void Treasure::Step(double delta_time){
    
}
