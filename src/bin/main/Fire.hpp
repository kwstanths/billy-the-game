#ifndef __Fire_hpp__
#define __Fire_hpp__

#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"

#include "Sun.hpp"


class Fire : public game_engine::WorldObject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::GameEngine * engine, Sun * sun);

    virtual void Step(double delta_time) override;

    virtual void Draw(game_engine::graphics::Renderer * renderer) override;

    virtual void Interact() override;

private:
    std::vector<game_engine::Real_t> attenutation_noise_;
    size_t index_;

    game_engine::graphics::PointLight_t light_;
    bool on_;

    Sun * sun_;
};

#endif