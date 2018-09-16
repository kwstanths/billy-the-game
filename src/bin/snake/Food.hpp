#ifndef __Food_hpp__
#define __Food_hpp__

#include <set>

#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/math/Types.hpp"


class Food : public game_engine::WorldObject {
public:
    
    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z,
        game_engine::WorldSector * world);

    void Step(double delta_time);

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    virtual void OnCollisionDetected(game_engine::physics::PhysicsObject * other) override;

    void MapAdd(game_engine::Real_t x, game_engine::Real_t y);

    void MapRemove(game_engine::Real_t x, game_engine::Real_t y);

private:

    bool spawn_food_ = false;

    std::vector<std::vector<bool>> food_map_;
    
    game_engine::math::Point2D GetArrayPoint(game_engine::Real_t x, game_engine::Real_t y);
    
};

#endif