#ifndef __World_hpp__
#define __World_hpp__

#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(game_engine::GameEngine * engine);

    int Destroy();


private:
    bool is_inited_;

};

#endif