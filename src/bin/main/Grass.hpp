#ifndef __Grass_hpp__
#define __Grass_hpp__

#include <string>

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

#include "MapProperties.hpp"

class Grass : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, std::string name,
        game_engine::WorldSector * world,
        game_engine::GameEngine * engine, MapProperties& map_properties);

private:
    std::string tile_name;
};

#endif