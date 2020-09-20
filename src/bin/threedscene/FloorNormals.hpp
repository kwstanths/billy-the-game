#ifndef __FloorNormals_hpp__
#define __FloorNormals_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/physics/PhysicsObject.hpp"

class FloorNormals : public game_engine::WorldObject {
public:
    FloorNormals();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world);

    virtual void Draw(game_engine::graphics::Renderer * render) override;

private:
    bool draw_ = false;
    bool is_inited_;
};


#endif