#include "FloorNormals.hpp"

namespace ge = game_engine;

#include "game_engine/core/ConsoleParser.hpp"
#include "game_engine/core/FileSystem.hpp"
#include "game_engine/graphics/Material.hpp"
#include "game_engine/math/Vector.hpp"

FloorNormals::FloorNormals() {
    is_inited_ = false;
}

int FloorNormals::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("plane2.obj", x, y, z);
    world->AddObject(this, x, y, z);

    //Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));
    
    std::string asssets_directory = ge::FileSystem::GetInstance().GetDirectoryAssets();

    ge::graphics::MaterialForwardDisplacementDrawNormals * material = new ge::graphics::MaterialForwardDisplacementDrawNormals(ge::math::Vector3D(1, 1, 0), asssets_directory + "textures/DoubleBasin_big.png");
    material->texture_displacement_->SetFiltering(GL_LINEAR);
    material->displacement_intensity_ = 10;

    SetMaterial(material, -1);

    is_inited_ = true;
    return ret == 0;
}

void FloorNormals::Draw(ge::graphics::Renderer * renderer) {

    ge::ConsoleCommand command = ge::ConsoleParser::GetInstance().GetLastCommand();

    if (command.type_  == COMMAND_TERRAIN_NORMALS && !ge::math::Equal(draw_, static_cast<bool>(command.arg_1_))) {
        draw_ = static_cast<bool>(command.arg_1_);
    }
    
    if (draw_)
        renderer->Draw(this);
}
