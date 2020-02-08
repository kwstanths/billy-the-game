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
#include "BidirectionalAstar.hpp"
#include "World.hpp"

class Particle : public game_engine::WorldObject {
public:
    
    enum IntegrationMethod { EulerOrig, EulerSemi, Verlet };

    struct SpringParticle {
        SpringParticle() : p_(nullptr), L_(0) {};
        SpringParticle(Particle * p, game_engine::Real_t L, game_engine::Real_t Ke) : p_(p), L_(L) {};

        Particle * p_;
        game_engine::Real_t L_;
    };


    Particle();

    int Init(std::vector<std::pair<int, int>> * empty_pos, game_engine::math::MersenneTwisterGenerator * rng,
        World * world, game_engine::utility::UniformGrid<int, 2> * grid, game_engine::GameEngine * engine);

    void Step(double delta_time) override;
    
    void Draw(game_engine::graphics::Renderer * renderer) override;

    int Destroy();

    void SetIntegrationMethod(IntegrationMethod m);
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

    glm::vec3 GetPositionCurrent();
    glm::vec3 GetPositionPrevious();
    glm::vec3 GetVelocity();
    glm::vec3 GetForce();
    game_engine::Real_t GetBouncing();
    game_engine::Real_t GetFriction();
    game_engine::Real_t GetLifetime();
    bool IsFixed();

    void Set1();
    void Set2();

    bool draw_spring_lines = false;
    
    glm::vec3 forward_, up_ = glm::vec3(0,1,0), side_;
private:
    bool is_inited_;

    IntegrationMethod method_ = IntegrationMethod::EulerSemi;
    game_engine::Real_t radius_ = 0.2;
    glm::vec3 position_current_;
    glm::vec3 position_previous_;
    glm::vec3 position_target_;
    glm::vec3 velocity_current_;
    glm::vec3 steering_velocity_;
    glm::vec3 desired_velocity_direction_;
    float max_velocity_norm_;
    glm::vec3 force_;
    float max_force_norm_;
    game_engine::Real_t bouncing_;
    game_engine::Real_t friction_;
    game_engine::Real_t lifetime_;
    game_engine::Real_t mass_;
    game_engine::Real_t verlet_k_;
    game_engine::Real_t spring_elasticity_k_;
    game_engine::Real_t spring_damping_k_;
    uint64_t last_waypoint_update_;
    bool fixed_;
    bool immortal_;
    bool is_paused_;

    game_engine::GameEngine * engine_;
    Input * input_;
    Camera * camera_;
    game_engine::math::MersenneTwisterGenerator * rng_;
    game_engine::ConsoleCommand last_command;

    game_engine::utility::UniformGrid<int, 2> * grid_;
    std::vector<std::pair<int, int>> * empty_pos_;
    World * world_;

    BidirectionalAstar astar;
    std::vector<BidirectionalAstar::CELL> path_;
    size_t waypoint_;

    glm::mat4 z_rotation_;

    glm::vec3 CorrectPlaneCollision(glm::vec3 normal, glm::vec3 velocity);
    void CheckWallCollisions();
    glm::vec3 GetInitialPosition();
    void Reset();
    void ResetHard();
    void SetAsarPath();
    bool SetDesiredVelocity();
    glm::vec3 Truncate(glm::vec3& input, float max_length);
};


#endif