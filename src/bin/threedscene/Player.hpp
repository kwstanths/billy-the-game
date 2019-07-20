#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/physics/PhysicsObject.hpp"

#include "Input.hpp"
#include "Camera.hpp"

class Player : public game_engine::WorldObject {
public:
    Player();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        Input * input, Camera * camera, 
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

    int Destroy();

private:
    bool is_inited_;

    game_engine::GameEngine * engine_;
    Input * input_;
    Camera * camera_;

};


#endif