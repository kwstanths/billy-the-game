#ifndef __StaticMap_hpp__
#define __StaticMap_hpp__

#include <string>

#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"
#include "game_engine/core/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

class StaticMap : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, std::string name,
        game_engine::WorldSector * world,
        game_engine::GameEngine * engine);

private:
};

#endif