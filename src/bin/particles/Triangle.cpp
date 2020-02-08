#include "Triangle.hpp"

namespace ge = game_engine;

TriangleObject::TriangleObject() {
    is_inited_ = false;
}

int TriangleObject::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world,
    glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    int ret = WorldObject::Init(x, y, z);
    world->AddObject(this, x, y, z);

    v1_ = v1;
    v2_ = v2;
    v3_ = v3;

    is_inited_ = true;
    return ret == 0;
}

void TriangleObject::Draw(game_engine::graphics::Renderer * renderer) {
    renderer->DrawTriangle(v2_, v1_, v3_, { 0.5, 0,0 });
}
