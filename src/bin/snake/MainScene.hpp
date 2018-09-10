#ifndef __MainScene_hpp__
#define __MainScene_hpp__


#include "game_engine/WorldSector.hpp"

#include "Input.hpp"
#include "Camera.hpp"

class MainScene : public game_engine::WorldSector {
public:
    MainScene();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();

private:
    bool is_inited_;
};

#endif