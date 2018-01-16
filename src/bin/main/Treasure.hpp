#ifndef __Treasure_hpp__
#define __Treasure_hpp__

#include "game_engine/WorldObject.hpp"

class Treasure : public game_engine::WorldObject {
public:

    virtual void Step(double delta_time) override;

private:

};

#endif