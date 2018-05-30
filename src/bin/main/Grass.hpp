#ifndef __Grass_hpp__
#define __Grass_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"

class Grass : public game_engine::WorldObject {
public:

    bool Init(game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

private:

};

#endif