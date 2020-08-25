#ifndef __WorldTavern1a_hpp__
#define __WorldTavern1a_hpp__

#include "World.hpp"

class WorldTavern1a : public World {
public:
    WorldTavern1a();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * billy_world);

    int Destroy();

private:
    bool is_inited_;
};

#endif
