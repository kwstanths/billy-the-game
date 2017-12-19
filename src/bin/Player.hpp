#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/WorldObject.hpp"


class Player : public game_engine::WorldObject{
public:
    Player();

    int Init();

    int Destroy();

    void Move(float move_x, float move_y);

    void Step(double delta_time);

    float GetSpeed(bool running);

private:
    bool is_inited_;

};


#endif