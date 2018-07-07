#ifndef __Lamp_hpp__
#define __Lamp_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/Material.hpp"

#include "Sun.hpp"


class Lamp : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine, Sun * sun);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

private:
    //float center_x_, center_y_;
    //float radius_;
    //float angular_speed_;

    std::vector<float> attenutation_noise_;
    size_t index_;

    game_engine::graphics::LightProperties_t light_;
    game_engine::graphics::Attenuation_t att_;
    
    Sun * sun_;
};

#endif