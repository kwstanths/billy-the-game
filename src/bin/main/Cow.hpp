#ifndef __Cow_hpp__
#define __Cow_hpp__

#include "game_engine/GameEngine.hpp"

#include "game_engine/WorldObject.hpp"

class Cow : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

    virtual void Interact() override;

private:

};

#endif