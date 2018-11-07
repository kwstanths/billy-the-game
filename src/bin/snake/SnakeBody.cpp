#include "SnakeBody.hpp"

#include "game_engine/math/Types.hpp"
namespace ge = game_engine;

bool SnakeBody::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {

    int ret = WorldObject::Init("assets/snake.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(0.95, 0.95, 0.95);
    SetCollision(ge::math::Rectangle2D(x, y, 0.95, 0.95));
    SetObjectType(1);

    return ret == 0;
}

void SnakeBody::Draw(game_engine::graphics::Renderer * renderer) {
    renderer->DrawSimple(this);
}

void SnakeBody::Move(game_engine::Real_t new_x, game_engine::Real_t new_y){
    SetPosition(new_x, new_y, GetZ());
}
