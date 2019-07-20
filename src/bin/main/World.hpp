#ifndef __World_hpp__
#define __World_hpp__

#include <vector>

#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "Sun.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();


private:
    bool is_inited_;

    void ReadMap(std::string name, float z, game_engine::GameEngine * engine);

    Sun * sun_;
};

#endif