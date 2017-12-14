#ifndef __Player_hpp__
#define __Player_hpp__

#include "game_engine/opengl/OpenGLObject.hpp"


class Player : public game_engine::OpenGLObject {
public:
    Player(game_engine::OpenGLObjectParams_t params);

    int Init();

    int Destroy();

    void Move(float move_x, float move_y);

private:
    bool is_inited_;
    float pos_x_, pos_y_, pos_z_;

};


#endif