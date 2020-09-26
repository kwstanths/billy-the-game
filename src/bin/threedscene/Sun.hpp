#ifndef __Sun_hpp__
#define __Sun_hpp__

#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/Light.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"

class Sun : public game_engine::WorldObject, public::game_engine::graphics::DirectionalLight {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

private:
    game_engine::GameEngine * engine_;

};

#endif