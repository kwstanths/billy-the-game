#ifndef __Path_hpp__
#define __Path_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"

class Path : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

private:

};

#endif