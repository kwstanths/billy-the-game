#ifndef __World_hpp__
#define __World_hpp__

#include "game_engine/core/WorldSector.hpp"
#include "game_engine/core/GameEngine.hpp"

#include "Input.hpp"
#include "Camera.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();


private:
    bool is_inited_;

};

#endif