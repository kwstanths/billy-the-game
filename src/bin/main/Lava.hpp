#ifndef __Lava_hpp__
#define __Lava_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"

class Player;

class Lava : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

private:

};

#endif