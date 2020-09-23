#include "Water.hpp"

namespace ge = game_engine;

#include "game_engine/graphics/Material.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/core/FileSystem.hpp"

Water::Water() {
    is_inited_ = false;
}

int Water::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("plane3.obj", x, y, z);
    world->AddObject(this, x, y, z);

    //Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));
    Scale(2, 2, 2);

    std::string asssets_directory = ge::FileSystem::GetInstance().GetDirectoryAssets();

    ge::graphics::MaterialForwardWater * material = new ge::graphics::MaterialForwardWater(ge::math::Vector3D(0, 0.2, 0.8), ge::math::Vector3D(0, 0.2, 0.8), ge::math::Vector3D(0.3, 0.5, 0.5), 32, asssets_directory + "textures/spec_map_empty.png", asssets_directory + "textures/spec_map_empty.png", asssets_directory + "textures/bump5.jpg");
    material->waves_.push_back(game_engine::graphics::Wave_t(ge::math::Vector3D(-1, 0, 0.01).ToGlm(), 35.5, 0.13));
    material->waves_.push_back(game_engine::graphics::Wave_t(ge::math::Vector3D(-1, 0, -1).ToGlm(), 25.5, 0.12));
    material->waves_.push_back(game_engine::graphics::Wave_t(ge::math::Vector3D(-1, 0, 1).ToGlm(), 25.5, 0.12));

    material->texture_bump_->SetFiltering(GL_LINEAR);

    SetMaterial(material, -1);

    is_inited_ = true;
    return ret == 0;
}

void Water::Draw(ge::graphics::Renderer * renderer) {
    renderer->Draw(this);
}
