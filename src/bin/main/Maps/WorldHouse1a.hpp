#ifndef __WorldHouse1a_hpp__
#define __WorldHouse1a_hpp__

#include "World.hpp"

class WorldHouse1a : public World {
public:
    WorldHouse1a();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * billy_world, World * second_floor);

    int Destroy();

private:
    bool is_inited_;
};

#endif
