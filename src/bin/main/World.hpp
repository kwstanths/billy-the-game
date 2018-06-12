#ifndef __World_hpp__
#define __World_hpp__

#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"

#include "Input.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(Input * input, game_engine::GameEngine * engine);

    int Destroy();


private:
    bool is_inited_;

};

#endif