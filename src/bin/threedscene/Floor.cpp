#include "Floor.hpp"

namespace ge = game_engine;

#include "game_engine/graphics/Material.hpp"
#include "game_engine/math/Vector.hpp"

Floor::Floor() {
    is_inited_ = false;
}

int Floor::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("assets/water.obj", x, y, z);
    world->AddObject(this, x, y, z);

    //Scale(50, 50, 50);
    //Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));

    ge::graphics::MaterialDeferredDisplacement * material = new ge::graphics::MaterialDeferredDisplacement(ge::math::Vector3D(0, 1, 0), ge::math::Vector3D(0, 1, 0), "assets/textures/world.png", "assets/textures/World_normal_map.png");
    material->texture_displacement_->SetFiltering(GL_LINEAR);
    material->texture_normal_->SetFiltering(GL_LINEAR);
    material->displacement_intensity_ = 12;

    SetMaterial(material, -1);

    is_inited_ = true;
    return ret == 0;
}

void Floor::Draw(ge::graphics::Renderer * renderer) {
    renderer->Draw(this);
}
