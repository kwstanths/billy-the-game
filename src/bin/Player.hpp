#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/WorldObject.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/Collision.hpp"

class Player : public game_engine::WorldObject{
public:
    Player();

    int Init();

    int Destroy();

    void Move(float move_offset, game_engine::CollisionResult_t input);

    void Step(double delta_time);

    float GetSpeed(bool running);

private:
    bool is_inited_;

    float speed_regular_, speed_running_;

};


#endif