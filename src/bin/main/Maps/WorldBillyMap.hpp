#ifndef __WorldBillyMap_hpp__
#define __WorldBillyMap_hpp__

#include "World.hpp"

class WorldBillyMap : public World {
public:
    WorldBillyMap();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine, World * tavern1_world, World * house1_world);

    int Destroy();

private:
    bool is_inited_;
};

#endif
