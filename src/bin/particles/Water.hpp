#ifndef __Water_hpp__
#define __Water_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/ConsoleParser.hpp"

class Water : public game_engine::WorldObject {
public:
    Water();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world);

    void Step(double delta_time) override;
    void Draw(game_engine::graphics::Renderer * renderer) override;

private:
    game_engine::ConsoleCommand last_command_;
    bool is_paused_ = false;

    bool is_inited_;
};


#endif