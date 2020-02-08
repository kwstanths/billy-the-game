#ifndef __Particle_hpp__
#define __Particle_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/math/RNG.hpp"
#include "game_engine/math/Real.hpp"
#include "game_engine/ConsoleParser.hpp"


#include "Input.hpp"
#include "Camera.hpp"
#include "CAGeometry.hpp"

class Particle : public game_engine::WorldObject {
public:
    
    enum IntegrationMethod { EulerOrig, EulerSemi, Verlet };
    enum SpawningMethod { Fountain, Waterfall, Semisphere, RAND_VELOCITY, NONE };

    struct SpringParticle {
        SpringParticle() : p_(nullptr), L_(0) {};
        SpringParticle(Particle * p, game_engine::Real_t L, game_engine::Real_t Ke) : p_(p), L_(L) {};

        Particle * p_;
        game_engine::Real_t L_;
    };


    Particle();

    int Init(SpawningMethod s, glm::vec3 position_spawn, game_engine::math::MersenneTwisterGenerator * rng,
        std::vector<Plane> * planes, std::vector<Triangle> * triangles,
        std::vector<Sphere> * spheres, game_engine::WorldSector * world, 
        game_engine::GameEngine * engine);

    void Step(double delta_time) override;
    
    void Draw(game_engine::graphics::Renderer * renderer) override;

    int Destroy();

    void SetIntegrationMethod(IntegrationMethod m);
    void SetSpawningMethod(SpawningMethod s);
    void SetPositionCurrent(glm::vec3 p);
    void SetPositionPrevious(glm::vec3 p);
    void SetVelocity(glm::vec3 p);
    void SetForce(glm::vec3 p);
    void AddForce(glm::vec3 p);
    void SetBouncing(game_engine::Real_t bouncing);
    void SetFriction(game_engine::Real_t friction);
    void SetLifetime(game_engine::Real_t seconds);
    void SetSpringElasticity(game_engine::Real_t elasticity);
    void SetSpringDamping(game_engine::Real_t damping);
    void SetFixed(bool fixed);
    void AddSpringForce(Particle * p, game_engine::Real_t spring_length, game_engine::Real_t elasticity);

    glm::vec3 GetPositionCurrent();
    glm::vec3 GetPositionPrevious();
    glm::vec3 GetVelocity();
    glm::vec3 GetForce();
    game_engine::Real_t GetBouncing();
    game_engine::Real_t GetFriction();
    game_engine::Real_t GetLifetime();
    bool IsFixed();

    bool draw_spring_lines = false;
    bool draw_particle = true;
    
private:
    bool is_inited_;

    IntegrationMethod method_ = IntegrationMethod::Verlet;
    SpawningMethod spawing_method_ = SpawningMethod::Fountain;
    glm::vec3 position_current_;
    glm::vec3 position_previous_;
    glm::vec3 position_spawn_;
    glm::vec3 velocity_;
    glm::vec3 force_;
    game_engine::Real_t bouncing_;
    game_engine::Real_t friction_;
    game_engine::Real_t lifetime_;
    game_engine::Real_t mass_;
    game_engine::Real_t verlet_k_;
    game_engine::Real_t spring_elasticity_k_;
    game_engine::Real_t spring_damping_k_;
    bool fixed_;
    bool immortal_;
    bool is_paused_;

    game_engine::GameEngine * engine_;
    Input * input_;
    Camera * camera_;
    game_engine::math::MersenneTwisterGenerator * rng_;
    game_engine::ConsoleCommand last_command;

    std::vector<Plane> * planes_ = nullptr;
    std::vector<game_engine::Real_t> plane_distances_;
    std::vector<Triangle> * triangles_ = nullptr;
    std::vector<game_engine::Real_t> triangle_distances_;
    std::vector<Sphere> * spheres_ = nullptr;
    std::vector<SpringParticle> spring_particles_;

    void CorrectPlaneCollision(Plane& plane, game_engine::Real_t& plane_distance);
    glm::vec3 GetInitialPosition(SpawningMethod s);
    void Reset();
    
    static glm::vec3 CalculateSpringForce(Particle * P1, Particle * P2, game_engine::Real_t L, game_engine::Real_t Ke, game_engine::Real_t Kd);
};


#endif