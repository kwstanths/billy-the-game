#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "Sun.hpp"

class Player : public game_engine::WorldObject {
public:
    Player();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        Input * input, Camera * camera,
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

    int Destroy();

    void Step(double delta_time);

    game_engine::Real_t GetSpeed(bool running);

    virtual void Draw(game_engine::graphics::Renderer * render);

    virtual void OnCollisionDetected(size_t type);

private:
    bool is_inited_;
    game_engine::Real_t radius_;
    game_engine::Real_t interact_fov_, interact_margin_;
    game_engine::Real_t looking_direction_;

    game_engine::GameEngine * engine_;
    Input * input_;
    Camera * camera_;
    Sun * sun_;

    game_engine::Real_t speed_regular_, speed_running_;
    ge::Real_t direction_array_[16] = { -1, 0.1, 180.1, -1, 270.1, 315.1, 225.1, -1, 90.1, 45.1, 135.1, -1, -1, -1, -1, -1 };

};


#endif