#include "SnakeHead.hpp"

#include "debug_tools/Console.hpp"
#include "SnakeBody.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool SnakeHead::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine, Input * input) {

    int ret = WorldObject::Init("assets/snake.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(0.2, 0.2, 0.2);
    SetCollision(ge::math::Rectangle2D(x, y, 0.2, 0.2));

    position_update_speed_ms_ = 200;
    ms_passed_ = 0;

    current_direction_ = MOVING::TOP;
    next_direction_ = MOVING::TOP;

    vertical_border_ = 2;
    horizontal_border_ = 2;

    body_tails_ = 0;

    input_ = input;

    SnakeBody * new_body = world_sector_->NewObj<SnakeBody>(true);
    new_body->Init(x, y-0.2, GetZ(), world_sector_);

    tail_.push_back(new_body);
    body_tails_++;

    return ret == 0;
}

void SnakeHead::Step(double delta_time) {

    ControlInput_t input = input_->GetControls();
    if (input.MOVE_LEFT_) next_direction_ = MOVING::LEFT;
    if (input.MOVE_RIGHT_) next_direction_ = MOVING::RIGHT;
    if (input.MOVE_UP_) next_direction_ = MOVING::TOP;
    if (input.MOVE_DOWN_) next_direction_ = MOVING::DOWN;

    if (input.INTERACT_) {
        SnakeBody * new_body = world_sector_->NewObj<SnakeBody>(true);
        new_body->Init(tail_[body_tails_-1]->GetX(), tail_[body_tails_-1]->GetY(), GetZ(), world_sector_);
        tail_.push_back(new_body);
        body_tails_++;
    }

    ms_passed_ += 1000.0 * delta_time;
    if (ms_passed_ >= position_update_speed_ms_) {
        
        if (current_direction_ == MOVING::TOP || current_direction_ == MOVING::DOWN)
            if (current_direction_ + next_direction_ != 4) current_direction_ = next_direction_;
        if (current_direction_ == MOVING::LEFT || current_direction_ == MOVING::RIGHT)
            if (current_direction_ + next_direction_ != 6) current_direction_ = next_direction_;
        
        float x = GetX();
        float y = GetY();

        float old_x = x;
        float old_y = y;
        if (current_direction_ == MOVING::TOP) {
            if (ge::math::Equal(y, vertical_border_, ge::Real_t(0.1))) y = -vertical_border_;
            else y += 0.2;
        }
        if (current_direction_ == MOVING::LEFT) {
            if (ge::math::Equal(x, -horizontal_border_, ge::Real_t(0.1))) x = horizontal_border_;
            else x -= 0.2;
        }
        if (current_direction_ == MOVING::DOWN) {
            if (ge::math::Equal(y, -vertical_border_, ge::Real_t(0.1))) y = vertical_border_;
            else y -= 0.2;
        }
        if (current_direction_ == MOVING::RIGHT) {
            if (ge::math::Equal(x, horizontal_border_, ge::Real_t(0.1))) x = -horizontal_border_;
            else x += 0.2;
        }
        SetPosition(x, y, GetZ(), ge::math::Rectangle2D(GetX(), GetY(), 0.5, 0.5));

        for (size_t i = tail_.size()-1; i >= 1; i--)
            tail_[i]->Move(tail_[i - 1]->GetX(), tail_[i - 1]->GetY());
        tail_[0]->Move(old_x, old_y);
        
        ms_passed_ = ge::Real_t(0);
    }

}

void SnakeHead::Draw(game_engine::graphics::Renderer * renderer) {

    renderer->DrawSimple(this);
    renderer->Draw2DText(std::to_string(tail_.size()), 0, 0, 1, { 255/255,69/255,0 });
}

void SnakeHead::OnCollisionDetected(size_t type) {
    for (size_t i = 1; i < tail_.size(); i++) {
        tail_[i]->Destroy();
    }
    tail_.resize(1);
    body_tails_ = 1;
}

