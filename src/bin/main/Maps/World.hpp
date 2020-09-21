#ifndef __World_hpp__
#define __World_hpp__

#include <vector>

#include "game_engine/core/WorldSector.hpp"
#include "game_engine/core/GameEngine.hpp"
#include "game_engine/utility/FIFOWorker.hpp"
#include "game_engine/math/AABox.hpp"

#include "TiledMap.hpp"
#include "MapProperties.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Sun.hpp"
#include "Fire.hpp"
#include "Player.hpp"

class World : public game_engine::WorldSector, public TiledMap {
public:
    World();

    int Init(Input * input, std::string map_name, game_engine::math::AABox<2> size, bool has_sun, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();

    Player * player_ = nullptr;

private:
    bool is_inited_;


    bool has_sun_;
    Sun * sun_ = nullptr;

    void ReadMap(std::string name, float z, game_engine::GameEngine * engine);
};

#endif