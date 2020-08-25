#include "WorldPortal.hpp"

#include "game_engine/graphics/opengl/OpenGLCamera.hpp"

#include "game_engine/math/AABox.hpp"

bool WorldPortal::Init(game_engine::math::Vector3D pos, game_engine::WorldSector * world, game_engine::WorldSector * other_world, game_engine::math::Vector3D other_pos, game_engine::GameEngine * engine, Camera * camera)
{
    engine_ = engine;
    other_world_ = other_world;
    position_ = other_pos;

    int ret = world->AddInterractableObject(this, game_engine::math::AABox<2>(Vector2D({ pos.x(), pos.y() }), { 1,1 }));

    return ret;
}

void WorldPortal::Interact()
{
    engine_->SetWorld(other_world_);
    camera->Set2DPosition(position_.x(), position_.y());
}
