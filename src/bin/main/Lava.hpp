#ifndef __Lava_hpp__
#define __Lava_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/math/Types.hpp"

class Lava : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

private:

};

#endif