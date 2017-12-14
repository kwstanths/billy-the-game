#include "Player.hpp"


Player::Player(game_engine::OpenGLObjectParams_t params): OpenGLObject(params) {

    Player::is_inited_ = false;
    pos_x_ = params.pos_x_;
    pos_y_ = params.pos_y_;
    pos_z_ = params.pos_z_;
}

int Player::Init() {

    return 0;
}

int Player::Destroy() {

    return 0;
}

void Player::Move(float move_x, float move_y) {
    pos_x_ += move_x;
    pos_y_ += move_y;
    
    OpenGLObject::SetPosition(pos_x_, pos_y_, pos_z_);
}

