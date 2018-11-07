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

    /**
        Add a snake position on the world
    */
    void MapAdd(game_engine::Real_t x, game_engine::Real_t y);

    /**
        Remove a snake position from the world 
    */
    void MapRemove(game_engine::Real_t x, game_engine::Real_t y);

private:
    /* Struct to hold two indices of the food_map_ data structure */
    struct map_indices_t {
        size_t i_;
        size_t j_;
        map_indices_t() : i_(0), j_(0) {};
        map_indices_t(size_t i, size_t j) : i_(i), j_(j) {};
    };

    /* If true, we have to spawn food */
    bool spawn_food_ = false;

    /**
        Data strucures for all the position in the grid, false = no snake there, true = snake there
    */
    std::vector<std::vector<bool>> snake_map_;
    
    /**
        Transform a real world position to a position in the food_map_
        @param pos The position
        @return food_map_ indices
    */
    Food::map_indices_t GetArrayPoint(game_engine::math::Point2D pos);

    /**
        Transform a food_map_ position to a real world position
        @param indices The food_map_ indices
        @return World coordiantes
    */
    game_engine::math::Point2D GetInvArrayPoint(Food::map_indices_t indices);
    
};

#endif