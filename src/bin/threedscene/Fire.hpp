#ifndef __Fire_hpp__
#define __Fire_hpp__

#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/Light.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"

class Fire : public::game_engine::graphics::PointLight {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z,
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

private:

};

#endif