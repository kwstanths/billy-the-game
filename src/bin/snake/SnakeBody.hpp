#ifndef __SnakeBody_hpp__
#define __SnakeBody_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

class SnakeBody : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z,
        game_engine::WorldSector * world);

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    void Move(game_engine::Real_t new_x, game_engine::Real_t new_y);

private:
    
};

#endif 
