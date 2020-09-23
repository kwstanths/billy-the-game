#include "Heightmap.hpp"

namespace ge = game_engine;

#include "game_engine/graphics/Material.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/core/FileSystem.hpp"

Heightmap::Heightmap() {
    is_inited_ = false;
}

int Heightmap::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world, std::string file) {
    int ret = WorldObject::Init("plane.obj", x, y, z);
    world->AddObject(this, x, y, z);

    //Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));
    Scale(0.5, 0.5, 0.5);

    std::string asssets_directory = ge::FileSystem::GetInstance().GetDirectoryAssets();

    ge::graphics::MaterialDeferredDisplacement * material = new ge::graphics::MaterialDeferredDisplacement(0, asssets_directory + file, asssets_directory + "textures/grass.png");
    material->texture_displacement_->SetFiltering(GL_LINEAR);
    material->displacement_intensity_ = 9;

    SetMaterial(material, -1);

    is_inited_ = true;
    return ret == 0;
}

void Heightmap::Draw(ge::graphics::Renderer * renderer) {
    renderer->Draw(this);
}
