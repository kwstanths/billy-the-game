#ifndef __Wall_hpp__
#define __Wall_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

class Wall : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        game_engine::WorldSector * world,
        game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

    virtual void Interact() override;

private:
    bool delete_object_ = false;
};

#endif