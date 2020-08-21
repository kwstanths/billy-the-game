#ifndef __World_hpp__
#define __World_hpp__

#include <vector>

#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/utility/FIFOWorker.hpp"

#include "MapProperties.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Sun.hpp"
#include "Fire.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();

private:
    bool is_inited_;

    MapProperties map_properties_;
    Sun * sun_;

    void ReadMap(std::string name, float z, game_engine::GameEngine * engine);
};

#endif