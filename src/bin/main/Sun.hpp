#ifndef __Sun_hpp__
#define __Sun_hpp__

#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/Light.hpp"
#include "game_engine/math/Types.hpp"


class Sun : public game_engine::WorldObject, public game_engine::graphics::DirectionalLight {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, 
        game_engine::WorldSector * world, game_engine::GameEngine * engine);

    virtual void StepLight(double delta_time) override;

    double GetTimeOfDay();

private:
    game_engine::Real_t day_period_;
    game_engine::Real_t game_time_ = 0.0f;
    game_engine::Real_t game_hour_ = 0.0f;

};

#endif