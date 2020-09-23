#ifndef __Heightmap_hpp__
#define __Heightmap_hpp__

#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"
#include "game_engine/core/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/physics/PhysicsObject.hpp"

class Heightmap : public game_engine::WorldObject {
public:
    Heightmap();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world, std::string file);

    virtual void Draw(game_engine::graphics::Renderer * render) override;

private:
    bool is_inited_;
};


#endif