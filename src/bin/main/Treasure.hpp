#ifndef __Treasure_hpp__
#define __Treasure_hpp__

#include "game_engine/GameEngine.hpp"

#include "game_engine/WorldObject.hpp"

class Treasure : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

private:

};

#endif