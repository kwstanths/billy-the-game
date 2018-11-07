#include "Food.hpp"

#include "game_engine/math/RNGenerator.hpp"

namespace ge = game_engine;

bool Food::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {

    int ret = WorldObject::Init("assets/food.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(0.95, 0.95, 0.1);
    SetCollision(ge::math::Rectangle2D(x, y, 0.95, 0.95));
    SetObjectType(2);

    snake_map_ = std::vector<std::vector<bool>>(21, std::vector<bool>(21, false));
    
    return true;
}

void Food::Step(double delta_time) {

}

void Food::Draw(game_engine::graphics::Renderer * renderer) {
    
    if (spawn_food_) {
        /* Find an empty position */
        size_t spaces = 0;
        std::vector<map_indices_t> empty_spaces(21 * 21);
        for (size_t i = 0; i < snake_map_.size(); i++) {
            for (size_t j = 0; j < snake_map_[i].size(); j++) {
                if (!snake_map_[i][j]) {
                    empty_spaces[spaces] = map_indices_t(i, j);
                    spaces++;
                }
            }
        }

        /* Add this object again to a random position */
        int rng = ge::math::RNGenerator::GetRand(0, spaces - 1);
        map_indices_t food_pos_indices = empty_spaces[rng];
        ge::math::Point2D food_pos = GetInvArrayPoint(food_pos_indices);
        world_sector_->AddObject(this, food_pos.x_, food_pos.y_, GetZ());

        spawn_food_ = false;
    }

    renderer->DrawSimple(this);
}

void Food::OnCollisionDetected(ge::physics::PhysicsObject * other) {
    /* collision detected, remove from the world */
    world_sector_->RemoveObject(this);
    spawn_food_ = true;
}

void Food::MapAdd(game_engine::Real_t x, game_engine::Real_t y) {

    map_indices_t map_indices = GetArrayPoint({ x, y });
    snake_map_[map_indices.i_][map_indices.j_] = true;
}

void Food::MapRemove(game_engine::Real_t x, game_engine::Real_t y) {

    map_indices_t map_indices = GetArrayPoint({ x, y });
    snake_map_[map_indices.i_][map_indices.j_] = false;
}

Food::map_indices_t Food::GetArrayPoint(ge::math::Point2D pos) {
    return { (10 - (size_t)pos.y_), ((size_t)pos.x_ + 10) };
}

game_engine::math::Point2D Food::GetInvArrayPoint(Food::map_indices_t indices) {
    return { ( ge::Real_t)indices.j_ - 10, 10 - (ge::Real_t)indices.i_ };
}
