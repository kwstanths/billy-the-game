#include "Food.hpp"

namespace ge = game_engine;

bool Food::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {

    int ret = WorldObject::Init("assets/food.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(0.15, 0.15, 0.15);
    SetCollision(ge::math::Rectangle2D(x, y, 0.15, 0.15));
    SetObjectType(2);

    return true;
}

void Food::Draw(game_engine::graphics::Renderer * renderer) {
    
    renderer->DrawSimple(this);
}

void Food::OnCollisionDetected(ge::physics::PhysicsObject * other) {
    Destroy();
}
