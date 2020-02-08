#include "Water.hpp"

#include "game_engine/ConsoleParser.hpp"

namespace ge = game_engine;

Water::Water() {
    is_inited_ = false;
}

int Water::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("assets/fplane4.obj", x, y, z);
    world->AddObject(this, x, y, z);

    is_inited_ = true;
    return ret == 0;
}

void Water::Step(double delta_time) {
    
}

void Water::Draw(game_engine::graphics::Renderer * renderer) {
    
    ge::ConsoleCommand c = game_engine::ConsoleParser::GetInstance().GetLastCommand();
    if (last_command_ != c) {
        last_command_ = c;
        if (c.type_ == COMMAND_CA_PAUSE) {
            is_paused_ = !is_paused_;
        }
    }
    if (is_paused_)
        renderer->DrawWater(this, false);
    else
        renderer->DrawWater(this);
}
