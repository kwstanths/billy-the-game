#include "Floor.hpp"

namespace ge = game_engine;

#include "game_engine/core/ConsoleParser.hpp"
#include "game_engine/core/FileSystem.hpp"
#include "game_engine/graphics/Material.hpp"
#include "game_engine/math/Vector.hpp"

Floor::Floor() {
    is_inited_ = false;
}

int Floor::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("grass.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(350, 350, 350);

    Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));

    is_inited_ = true;
    return ret == 0;
}

void Floor::Draw(ge::graphics::Renderer * renderer) {
    renderer->Draw(this);
}
