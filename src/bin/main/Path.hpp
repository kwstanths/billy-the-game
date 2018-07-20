#ifndef __Path_hpp__
#define __Path_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/math/Types.hpp"

class Path : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::GameEngine * engine);

private:

};

#endif