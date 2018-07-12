#ifndef __Fire_hpp__
#define __Fire_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"

#include "Sun.hpp"


class Fire : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine, Sun * sun);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

private:
    std::vector<float> attenutation_noise_;
    size_t index_;

    game_engine::graphics::LightProperties_t light_;
    game_engine::graphics::Attenuation_t att_;
    
    Sun * sun_;
};

#endif