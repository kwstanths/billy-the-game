#ifndef __WorldHouse1bhpp__
#define __WorldHouse1b_hpp__

#include "World.hpp"

class WorldHouse1b : public World {
public:
    WorldHouse1b();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * first_floor);

    int Destroy();

private:
    bool is_inited_;
};

#endif
