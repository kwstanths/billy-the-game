#ifndef __Food_hpp__
#define __Food_hpp__

#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"


class Food : public game_engine::WorldObject {
public:
    
    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z,
        game_engine::WorldSector * world);

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    virtual void OnCollisionDetected(game_engine::physics::PhysicsObject * other) override;

private:

};

#endif