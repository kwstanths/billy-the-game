#ifndef __Treasure_hpp__
#define __Treasure_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/math/Types.hpp"

class Treasure : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::GameEngine * engine, size_t id);

    virtual void Step(double delta_time) override;

    virtual void Interact() override;

private:
    size_t id_;
};

#endif