#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"

#include "Input.hpp"
#include "Camera.hpp"

class Player : public game_engine::WorldObject {
public:
    Player();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();

    void Step(double delta_time);

    game_engine::Real_t GetSpeed(bool running);

    virtual void Draw(game_engine::graphics::Renderer * render);

private:
    bool is_inited_;
    game_engine::Real_t radius_;
    game_engine::Real_t interact_fov_, interact_margin_;
    game_engine::Direction_t looking_direction_;

    game_engine::GameEngine * engine_;
    Input * input_;
    Camera * camera_;

    game_engine::Real_t speed_regular_, speed_running_;
    ge::Direction_t direction_array_[16] = { -1, 270, 90, -1, 180, 225, 135, -1, 0, 315, 45, -1, -1, -1, -1, -1 };

};


#endif