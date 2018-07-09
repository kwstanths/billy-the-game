#ifndef __Sun_hpp__
#define __Sun_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/Material.hpp"


class Sun : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    double GetTimeOfDay();

private:
    float day_period_;
    float game_time_;
    float game_hour_;
    game_engine::graphics::LightProperties_t light_;

};

#endif