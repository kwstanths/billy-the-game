#ifndef __Sphere_hpp__
#define __Sphere_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vector.hpp"

#include "Particle.hpp"

class SphereObject : public game_engine::WorldObject {
public:
    SphereObject();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::Real_t radius, game_engine::WorldSector * world);

    void Step(double delta_time) override;
    void Draw(game_engine::graphics::Renderer * renderer) override;

    Particle::IntegrationMethod method_;
    game_engine::Real_t bouncing_;
    game_engine::Real_t friction_;
    game_engine::Real_t spring_elasticity_k_;
    game_engine::Real_t spring_damping_k_;

    std::vector<Particle *> cloth_triangles_;

    bool draw_spring_triangles_ = false;
private:

    bool is_inited_;
};


#endif