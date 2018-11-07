#include "SnakeHead.hpp"

#include "debug_tools/Console.hpp"
#include "SnakeBody.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


bool SnakeHead::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, Input * input, ge::Real_t update_time, Food * food) {

    int ret = WorldObject::Init("assets/snake.obj", x, y, z);
    world->AddObject(this, x, y, z);

    /* Everything is of 0.2 size */
    SetCollision(ge::math::Rectangle2D(x, y, 1, 1));
    SetObjectType(1);

    position_update_speed_ms_ = update_time;
    ms_passed_ = 0;

    current_direction_ = MOVING::TOP;
    next_direction_ = MOVING::TOP;

    /* This is hardcoded here */
    vertical_border_ = 10;
    horizontal_border_ = 10;

    body_tails_ = 0;

    input_ = input;
    food_ = food;

    /* Add one body to the tail */
    SnakeBody * new_body = world_sector_->NewObj<SnakeBody>(true);
    new_body->Init(x, y-1, GetZ(), world_sector_);

    /* Add these position to the food snake map */
    food_->MapAdd(x, y);
    food_->MapAdd(x, y - 1);

    /* Update tail */
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

    ms_passed_ += 1000.0 * delta_time;
    if (ms_passed_ >= position_update_speed_ms_) {
        /* Check for valid move, we can't do a 180 degrees direction change */
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
            else y += 1;
        }
        if (current_direction_ == MOVING::LEFT) {
            if (ge::math::Equal(x, -horizontal_border_, ge::Real_t(0.1))) x = horizontal_border_;
            else x -= 1;
        }
        if (current_direction_ == MOVING::DOWN) {
            if (ge::math::Equal(y, -vertical_border_, ge::Real_t(0.1))) y = vertical_border_;
            else y -= 1;
        }
        if (current_direction_ == MOVING::RIGHT) {
            if (ge::math::Equal(x, horizontal_border_, ge::Real_t(0.1))) x = -horizontal_border_;
            else x += 1;
        }

        bool ret = SetPosition(x, y, GetZ(), ge::math::Rectangle2D(GetX(), GetY(), 1, 1));
        food_->MapAdd(x, y);

        /* If no collision, adance the tail */
        if (!ret) AdvanceTail(old_x, old_y);
        
        ms_passed_ = ge::Real_t(0);
    }

}

void SnakeHead::Draw(game_engine::graphics::Renderer * renderer) {

    /* Simple drawing, otherwise, everything whould be black */
    renderer->DrawSimple(this);
    /* Draw the score as well */
    renderer->Draw2DText(std::to_string(tail_.size()), 0, 0, 1, { 255/255,69/255,0 });
}

void SnakeHead::OnCollisionDetected(ge::physics::PhysicsObject * other) {

    size_t type = other->GetObjectType();

    switch (type)
    {
    case 1:
    {
        /* We hit ourselves! */
        for (size_t i = 1; i < tail_.size(); i++) {
            tail_[i]->Destroy();
        }
        tail_.resize(1);
        body_tails_ = 1;

        break;
    }
    case 2:
    {
        /* We hit food! */
        float old_x = GetX();
        float old_y = GetY();
        /* "Eat" food */
        SetPosition(other->GetX(), other->GetY(), other->GetZ());
     
        /* Advance tail */
        AdvanceTail(old_x, old_y);

        /* Spawn new body */
        SnakeBody * new_body = world_sector_->NewObj<SnakeBody>(true);
        new_body->Init(tail_[body_tails_ - 1]->GetX(), tail_[body_tails_ - 1]->GetY(), GetZ(), world_sector_);
        tail_.push_back(new_body);
        body_tails_++;

        break;
    }
    }

}

void SnakeHead::AdvanceTail(ge::Real_t old_x, ge::Real_t old_y) {

    /* 
        When food is eaten, the new snake body is placed on "top" of the last snake body
        So in the next frame, when the positions will be updated and the tail will be unfolded,
        the tail is occupied from two snake bodies. One of them will move, and the last will 
        stay in the same position, giving the feeling of the snake "growing". For this reason,
        in order for the food map to be correct, we check if the tail has two bodies, so that we
        won't remove it from the food map
    */

    if (tail_.size() > 1) {
        SnakeBody * last = tail_[tail_.size() - 1];
        SnakeBody * second_to_last = tail_[tail_.size() - 2];
        /* Only update food map, when the last two dont share the same position */
        if (!ge::math::Equal(last->GetX(), second_to_last->GetX()) || !ge::math::Equal(last->GetY(), second_to_last->GetY())) {
            food_->MapRemove(last->GetX(), last->GetY());
        }
    } else {
        SnakeBody * last = tail_[tail_.size() - 1];
        food_->MapRemove(last->GetX(), last->GetY());
    }

    for (size_t i = tail_.size() - 1; i >= 1; i--)
        tail_[i]->Move(tail_[i - 1]->GetX(), tail_[i - 1]->GetY());
    
    tail_[0]->Move(old_x, old_y);
}

