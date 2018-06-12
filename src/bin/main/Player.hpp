#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/physics/Collision.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "Input.hpp"

class Player : public game_engine::WorldObject {
public:
    Player();

    int Init(float x, float y, float z, Input * input, game_engine::GameEngine * engine);

    int Destroy();

    void Move(float move_offset, game_engine::CollisionResult_t collision_input);

    void Step(double delta_time);

    float GetSpeed(bool running);

private:
    bool is_inited_;
    float radius_;
    float interact_fov_, interact_margin_;
    game_engine::GameEngine * engine_;
    Input * input_;

    float speed_regular_, speed_running_;
    float direction_array_[16] = { -1, 270, 90, -1, 180, 225, 135, -1, 0, 315, 45, -1, -1, -1, -1, -1 };

};


#endif