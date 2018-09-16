#include "Food.hpp"

#include "game_engine/math/RNGenerator.hpp"

namespace ge = game_engine;

bool Food::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {

    int ret = WorldObject::Init("assets/food.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(0.15, 0.15, 0.15);
    SetCollision(ge::math::Rectangle2D(x, y, 0.15, 0.15));
    SetObjectType(2);

    food_map_ = std::vector<std::vector<bool>>(21, std::vector<bool>(21, false));
    
    return true;
}

void Food::Step(double delta_time) {

    int rng = ge::math::RNGenerator::GetRand(0, 441);

}

void Food::Draw(game_engine::graphics::Renderer * renderer) {
    
    renderer->DrawSimple(this);
}

void Food::OnCollisionDetected(ge::physics::PhysicsObject * other) {
    world_sector_->RemoveObject(this);

    spawn_food_ = true;
}

void Food::MapAdd(game_engine::Real_t x, game_engine::Real_t y) {
    ge::math::Point2D map_indices = GetArrayPoint(x, y);

    food_map_[map_indices.x_][map_indices.y_] = true;
}

void Food::MapRemove(game_engine::Real_t x, game_engine::Real_t y) {
    ge::math::Point2D map_indices = GetArrayPoint(x, y);

    food_map_[map_indices.x_][map_indices.y_] = false;
}

ge::math::Point2D Food::GetArrayPoint(game_engine::Real_t x, game_engine::Real_t y) {
    return { (-y + 2) * 5, (x + 2) * 5 };
}
