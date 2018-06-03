#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/Collision.hpp"

class Player : public game_engine::WorldObject {
public:
    Player();

    int Init(game_engine::GameEngine * engine);

    int Destroy();

    void Move(float move_offset, ControlInput control_input, game_engine::CollisionResult_t collision_input);

    void Step(double delta_time);

    float GetSpeed(bool running);

private:
    bool is_inited_;
    game_engine::GameEngine * engine_;

    float speed_regular_, speed_running_;

};


#endif