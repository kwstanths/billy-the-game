#include "Particle.hpp"

#include <ctime>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/ConsoleParser.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace grph = game_engine::graphics;
namespace math = game_engine::math;

using namespace game_engine;

Particle::Particle(): WorldObject() {
    engine_ = nullptr;
    is_inited_ = false;

    srand(time(NULL));

    position_current_ = glm::vec3(0);
    position_previous_ = glm::vec3(0);
    velocity_ = glm::vec3(0);
    force_ = glm::vec3(0);
    mass_ = Real_t(1);
    
    bouncing_ = Real_t(0.3);
    
    friction_ = Real_t(0.0);

    verlet_k_ = Real_t(0.99);
    spring_damping_k_ = Real_t(0.2);
    spring_elasticity_k_ = Real_t(30);

    lifetime_ = std::numeric_limits<Real_t>::max();
    fixed_ = false;
    immortal_ = true;
    is_paused_ = false;
}

int Particle::Init(SpawningMethod s, glm::vec3 position_spawn, game_engine::math::MersenneTwisterGenerator * rng,
    std::vector<Plane> * planes, std::vector<Triangle> * triangles, std::vector<Sphere> * spheres, 
    ge::WorldSector * world, ge::GameEngine * engine)
{

    engine_ = engine;
    planes_ = planes;
    triangles_ = triangles;
    spheres_ = spheres;
    rng_ = rng;
    spawing_method_ = s;
    position_spawn_ = position_spawn;

    glm::vec3 spos = GetInitialPosition(s);

    int ret = WorldObject::Init("assets/particle.obj", spos.x, spos.y, spos.z);
    world->AddObject(this, spos.x, spos.y, spos.z);

    position_current_ = glm::vec3({ spos.x, spos.y, spos.z });

    Scale(0.05f, 0.05f, 0.05f);

    plane_distances_.resize(planes_->size(), Real_t(0));
    for (size_t i = 0; i < planes_->size(); i++) {
        plane_distances_[i] = planes_->at(i).distPoint2Plane(glm::vec3(position_current_[0], position_current_[1], position_current_[2]));
    }

    triangle_distances_.resize(triangles_->size(), Real_t(0));
    for (size_t i = 0; i < triangles_->size(); i++) {
        triangle_distances_[i] = triangles_->at(i).distPoint2Plane(glm::vec3(position_current_[0], position_current_[1], position_current_[2]));
    }

    is_inited_ = true;
    return ret == 0;
}

void Particle::Step(double delta_time) {

    /* Clamp delta time, mostly for the first few frames */
    delta_time = ge::math::clamp(delta_time, 0.0, 0.017);

    /* Parse last command */
    ge::ConsoleCommand c = ConsoleParser::GetInstance().GetLastCommand();
    if (last_command != c) {
        last_command = c;
        if (c.type_ == COMMAND_CA_RESET) {
            Reset();
            return;
        } else if (c.type_ == COMMAND_CA_PAUSE){
            is_paused_ = !is_paused_;
            return;
        } else if (c.type_ == COMMAND_CA_SPAWN) {
            if (static_cast<int>(c.arg_1_) < 4) {
                spawing_method_ = SpawningMethod(static_cast<int>(c.arg_1_));
                Reset();
                return;
            }
        } else if (c.type_ == COMMAND_CA_INTEGRATION) {
            if (static_cast<int>(c.arg_1_) < 3) {
                method_ = IntegrationMethod(static_cast<int>(c.arg_1_));
            }
        } else if (c.type_ == COMMAND_CA_BOUNCING) {
            bouncing_ = ge::math::clamp(c.arg_1_, 0.0f, 1.0f);
        } else if (c.type_ == COMMAND_CA_FRICTION) {
            friction_ = ge::math::clamp(c.arg_1_, 0.0f, 1.0f);
        } else if (c.type_ == COMMAND_CA_ELASTICITY) {
            spring_elasticity_k_ = c.arg_1_;
        } else if (c.type_ == COMMAND_CA_DAMPING) {
            spring_damping_k_ = c.arg_1_;
        } else if (c.type_ == COMMAND_CA_DRAW_PARTICLES) {
            draw_particle = static_cast<bool>(c.arg_1_);
        } else if (c.type_ == COMMAND_CA_DRAW_SPRING_LINES) {
            draw_spring_lines = static_cast<bool>(c.arg_1_);
        }
    }

    if (fixed_ || is_paused_) {
        WorldObject::SetPosition(position_current_[0], position_current_[1], position_current_[2]);
        return;
    }

    /* Check lifetime */
    lifetime_ -= delta_time;
    if (lifetime_ < 0) {
        
        Reset();
        return;
    }

    /* Calcualte spring forces */
    glm::vec3 spring_forces = glm::vec3(0, 0, 0);
    for (size_t i = 0; i < spring_particles_.size(); i++) {
        spring_forces += CalculateSpringForce(this, spring_particles_[i].p_, spring_particles_[i].L_, spring_elasticity_k_, spring_damping_k_);
    }

    /* Update position */
    switch (method_)
    {
    case IntegrationMethod::EulerOrig:
    {
        position_previous_ = position_current_;
        position_current_ += velocity_ * float(delta_time);
        velocity_ += ((force_ + spring_forces) / mass_) * float(delta_time);
        
        break;
    }
    case IntegrationMethod::EulerSemi:
    {
        position_previous_ = position_current_;
        velocity_ += ((force_ + spring_forces) / mass_) * (float)delta_time;
        position_current_ += velocity_ * (float)delta_time;
        
        break;
    }
    case IntegrationMethod::Verlet:
    {
        if (ge::math::Equal(position_current_.x, position_previous_.x) && ge::math::Equal(position_current_.y, position_previous_.y) && ge::math::Equal(position_current_.z, position_previous_.z)) {
            velocity_ += ((force_ + spring_forces) / mass_) * (float)delta_time;
            position_current_ += velocity_ * (float)delta_time;
        } else {
            glm::vec3 temp = position_current_;
            position_current_ = position_current_ + verlet_k_ * (position_current_ - position_previous_) + float(delta_time * delta_time) * ((force_ + spring_forces)/ mass_);
            velocity_ = (position_current_ - temp) / ((float) delta_time);
            position_previous_ = temp;
        }

        break;
    }
    }

    /* Calculate plane collisions */
    for (size_t i = 0; i < planes_->size(); i++) {
        Real_t prev_distance = plane_distances_[i];
        plane_distances_[i] = planes_->at(i).distPoint2Plane(glm::vec3(position_current_[0], position_current_[1], position_current_[2]));
        if (prev_distance * plane_distances_[i] < 0.0f) {
            CorrectPlaneCollision(planes_->at(i), plane_distances_[i]);
            position_previous_ = position_current_ - float(delta_time) * velocity_;
        }
    }

    /* Calcualte triangle collisions */
    for (size_t i = 0; i < triangles_->size(); i++) {
        bool ret = triangles_->at(i).isInside(position_current_);
        Real_t prev_distance = triangle_distances_[i];
        triangle_distances_[i] = triangles_->at(i).distPoint2Plane(glm::vec3(position_current_[0], position_current_[1], position_current_[2]));
        if (ret && (prev_distance * triangle_distances_[i] < 0.0f)) {
            CorrectPlaneCollision(triangles_->at(i), triangle_distances_[i]);
            position_previous_ = position_current_ - float(delta_time) * velocity_;
        }
    }

    /* Calculate sphere collisions */
    for (size_t i = 0; i < spheres_->size(); i++) {
        bool ret = spheres_->at(i).isInside(position_current_);
        if (ret) {
            glm::vec3 P;
            bool ret = spheres_->at(i).intersecSegment(position_previous_, position_current_, P);
            if (!ret) break;

            float temp;
            CorrectPlaneCollision(Plane(P, P - spheres_->at(i).center), temp);
            position_previous_ = position_current_ - float(delta_time) * velocity_;
        }
    }
        
    /* Update position */
    if (std::abs(position_current_[0]) > 10 || std::abs(position_current_[2]) > 10) {
        dt::ConsoleInfoL(dt::WARNING, "Particle out of bounds", 
            "position_current", position_current_,
            "position_previous", position_previous_,
            "velocity", velocity_
        );
        Reset();
    } else {
        WorldObject::SetPosition(position_current_[0], position_current_[1], position_current_[2]);
    }

    return;
}

void Particle::Draw(game_engine::graphics::Renderer * renderer) {
    /* Draw a line for each spring connection */
    if (draw_spring_lines) {
        for (size_t i = 0; i < spring_particles_.size(); i++) {
            renderer->DrawLine(Vector3D(position_current_), Vector3D(spring_particles_[i].p_->GetPositionCurrent()), 3, glm::vec3(1, 1, 1));
        }
    }

    float average_water_displacement = renderer->GetAverageWaterDisplacement();
    if (draw_particle && (position_current_.y < (1.1f + average_water_displacement)) && (position_previous_.y > (1.1f + average_water_displacement))) {
        float x = ge::math::map_to_range(-5.0f, 5.0f, position_current_.x, 0.0f, 1.0f);
        float y = ge::math::map_to_range(-5.0f, 5.0f, position_current_.z, 0.0f, 1.0f);
        renderer->AddDropplet(glm::vec2(x, y));
    }

    if (draw_particle) 
        renderer->Draw(this);
}

void Particle::CorrectPlaneCollision(Plane& plane, Real_t& plane_distance) {
    glm::vec3 n = plane.normal;
    float d = plane.dconst;

    glm::vec3 velocity_t = velocity_;
    /* Update position, apply elasticity */
    position_current_ = position_current_ - (Real_t(1 + bouncing_)) * (glm::dot(n, position_current_) + d) * n;
    velocity_ = velocity_ - (Real_t(1 + bouncing_)) * (glm::dot(n, velocity_)) * n;

    /* Update velocity with friction force */
    velocity_ = velocity_ - friction_ * (velocity_t - glm::dot(n, velocity_t) * n);

    plane_distance = -plane_distance;
}

glm::vec3 Particle::GetInitialPosition(SpawningMethod s)
{
    switch (s)
    {
    case Particle::Fountain:
    {
        position_current_ = glm::vec3(0, 4.2, 0);
        velocity_ = glm::vec3(6 * (rng_->genrand_real3() - 0.5f), 1 + rng_->genrand_real3(), 6 * (rng_->genrand_real3() - 0.5f));
        position_previous_ = position_current_ - 0.016f * velocity_;
        return position_current_;
    }
    case Particle::Waterfall:
    {
        position_current_ = glm::vec3(rng_->genrand_real3() - 0.5f, 10, rng_->genrand_real3() - 0.5f);
        velocity_ = glm::vec3(rng_->genrand_real3() - 0.5f, -4 + rng_->genrand_real3(), rng_->genrand_real3() - 0.5f);
        position_previous_ = position_current_ - 0.016f * velocity_;
        return position_current_;
    }
    case Particle::Semisphere: 
    {
        float a = 360.0f * (rng_->genrand_real3() - 0.5f);
        float b = 90.0f * rng_->genrand_real3();

        position_current_ = glm::vec3(0, 2.5, 0) + glm::vec3(2 * glm::cos(a) * glm::cos(b), 2* glm::sin(b), 2*glm::sin(a) * glm::cos(b));
        velocity_ = glm::normalize(position_current_) * glm::vec3(6, 6, 6);
        position_previous_ = position_current_ - 0.016f * velocity_;
        return position_current_;
    }
    case Particle::RAND_VELOCITY: {
        position_current_ = position_spawn_;
        position_previous_ = position_current_;
        if (rng_->genrand_real1() < 0.1) 
            velocity_ = glm::vec3(rng_->genrand_real1() - 0.5f, rng_->genrand_real1() - 0.5f, rng_->genrand_real1() - 0.5f) * 10.0f;
        else 
            velocity_ = glm::vec3(0, 0, 0);

        return position_current_;
        break;
    }
    default:
        position_current_ = position_spawn_;
        position_previous_ = position_current_;
        velocity_ = glm::vec3(0, 0, 0);
        return position_current_;
        break;
    }
}

void Particle::Reset() {
    glm::vec3 initial_position = GetInitialPosition(spawing_method_);

    for (size_t i = 0; i < plane_distances_.size(); i++) {
        plane_distances_[i] = planes_->at(i).distPoint2Plane(glm::vec3(initial_position[0], initial_position[1], initial_position[2]));
    }
    for (size_t i = 0; i < triangle_distances_.size(); i++) {
        triangle_distances_[i] = triangles_->at(i).distPoint2Plane(glm::vec3(initial_position[0], initial_position[1], initial_position[2]));
    }

    WorldObject::SetPosition(initial_position[0], initial_position[1], initial_position[2]);
    lifetime_ = (immortal_) ? std::numeric_limits<Real_t>::max() : (2.0f + rng_->genrand_real3() * 8.0f);
}

glm::vec3 Particle::CalculateSpringForce(Particle * P1, Particle * P2, Real_t L, Real_t Ke, Real_t Kd) {
    glm::vec3 p1 = P1->GetPositionCurrent();
    glm::vec3 p2 = P2->GetPositionCurrent();
    glm::vec3 v1 = P1->GetVelocity();
    glm::vec3 v2 = P2->GetVelocity();
    
    glm::vec3 p2p1 = p2 - p1;
    glm::vec3 p2p1_dir = glm::normalize(p2p1);

    return (Ke * (glm::length(p2p1) - L) + Kd * glm::dot(v2 - v1, p2p1_dir)) * p2p1_dir;
}

int Particle::Destroy() {

    return 0;
}

void Particle::SetIntegrationMethod(IntegrationMethod m) {
    method_ = m;
}

void Particle::SetSpawningMethod(SpawningMethod s)
{
    spawing_method_ = s;
}

void Particle::SetPositionCurrent(glm::vec3 p) {
    position_current_ = p;
}

void Particle::SetPositionPrevious(glm::vec3 p) {
    position_previous_ = p;
}

void Particle::SetVelocity(glm::vec3 p) {
    velocity_ = p;
    position_previous_ = position_current_ - 0.016f * velocity_;
}

void Particle::SetForce(glm::vec3 p) {
    force_ = p;
}

void Particle::AddForce(glm::vec3 p) {
    force_ += p;
}

void Particle::SetBouncing(game_engine::Real_t bouncing) {
    bouncing_ = bouncing;
}

void Particle::SetFriction(game_engine::Real_t friction) {
    friction_ = friction;
}

void Particle::SetLifetime(game_engine::Real_t seconds) {
    lifetime_ = seconds;
    immortal_ = false;
}

void Particle::SetSpringElasticity(game_engine::Real_t elasticity)
{
    spring_elasticity_k_ = elasticity;
}

void Particle::SetSpringDamping(game_engine::Real_t damping)
{
    spring_damping_k_ = damping;
}

void Particle::SetFixed(bool fixed) {
    fixed_ = fixed;
}

void Particle::AddSpringForce(Particle * p, Real_t spring_length, Real_t elasticity) {
    spring_particles_.push_back(SpringParticle(p, spring_length, elasticity));
}

glm::vec3 Particle::GetPositionCurrent()
{
    return position_current_;
}

glm::vec3 Particle::GetPositionPrevious()
{
    return position_previous_;
}

glm::vec3 Particle::GetVelocity()
{
    return velocity_;
}

glm::vec3 Particle::GetForce()
{
    return force_;
}

game_engine::Real_t Particle::GetBouncing()
{
    return bouncing_;
}

game_engine::Real_t Particle::GetFriction()
{
    return friction_;
}

game_engine::Real_t Particle::GetLifetime() {
    return lifetime_;
}

bool Particle::IsFixed()
{
    return fixed_;
}
