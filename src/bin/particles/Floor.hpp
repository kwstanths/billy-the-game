#ifndef __Floor_hpp__
#define __Floor_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vector.hpp"

class Floor : public game_engine::WorldObject {
public:
    Floor();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world);

private:
    bool is_inited_;
};


#endif