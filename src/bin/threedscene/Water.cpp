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

    ge::graphics::MaterialForwardWater * material = new ge::graphics::MaterialForwardWater(ge::math::Vector3D(0, 0, 0.6), ge::math::Vector3D(0, 0.4, 0.6), ge::math::Vector3D(0.5, 0.05, 0.05), 16, asssets_directory + "textures/spec_map_empty.png", asssets_directory + "textures/spec_map_empty.png");

    SetMaterial(material, -1);

    is_inited_ = true;
    return ret == 0;
}

void Water::Draw(ge::graphics::Renderer * renderer) {
    renderer->Draw(this);
}
