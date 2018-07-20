#ifndef __Sun_hpp__
#define __Sun_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/math/Types.hpp"


class Sun : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    double GetTimeOfDay();

private:
    game_engine::Real_t day_period_;
    game_engine::Real_t game_time_;
    game_engine::Real_t game_hour_;
    game_engine::graphics::LightProperties_t light_;

};

#endif