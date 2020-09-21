#ifndef __Fire_hpp__
#define __Fire_hpp__

#include "game_engine/graphics/Light.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"

#include "Sun.hpp"

/**
    Represents the torch object in the world, It is a point a light, and an interactable object
*/
class Fire : public game_engine::graphics::PointLight, game_engine::Interactablebject {
public:

    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, float intensity,
        game_engine::WorldSector * world, game_engine::GameEngine * engine, Sun * sun);

    virtual void StepLight(double delta_time) override;

    virtual void Interact() override;

private:
    std::vector<game_engine::Real_t> attenutation_noise_;
    size_t index_;
    glm::vec3 diffuse_value_;

    bool on_;

    Sun * sun_;
};

#endif