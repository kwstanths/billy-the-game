#ifndef __Background_hpp__
#define __Background_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"


class Background : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

private:

};

#endif