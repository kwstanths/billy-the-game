#ifndef __WorldTavern1b_hpp__
#define __WorldTavern1b_hpp__

#include "World.hpp"

class WorldTavern1b : public World {
public:
    WorldTavern1b();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * billy_world);

    int Destroy();

private:
    bool is_inited_;
};

#endif
