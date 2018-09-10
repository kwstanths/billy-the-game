#ifndef __SnakeHead_hpp__
#define __SnakeHead_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

#include "Input.hpp"
#include "SnakeBody.hpp"

class SnakeHead : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z,
        game_engine::WorldSector * world, game_engine::GameEngine * engine, Input * input);

    void Step(double delta_time);

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    virtual void OnCollisionDetected(size_t type);

private:

    enum MOVING {
        TOP = 1,
        LEFT = 2,
        DOWN = 3,
        RIGHT = 4,
    };

    Input * input_;

    game_engine::Real_t position_update_speed_ms_;
    game_engine::Real_t ms_passed_;

    MOVING current_direction_;
    MOVING next_direction_;

    game_engine::Real_t vertical_border_;
    game_engine::Real_t horizontal_border_;

    size_t body_tails_;
    std::vector<SnakeBody *> tail_;
};

#endif