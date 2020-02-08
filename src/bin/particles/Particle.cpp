#include "Particle.hpp"

#include <ctime>
#include <algorithm>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Matrices.hpp"
#include "game_engine/math/AABox.hpp"
#include "game_engine/math/Geometry.hpp"
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
    velocity_current_ = glm::vec3(0);
    max_velocity_norm_ = 2;
    force_ = glm::vec3(0);
    max_force_norm_ = 4;
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

int Particle::Init(std::vector<std::pair<int, int>> * empty_pos, game_engine::math::MersenneTwisterGenerator * rng,
    World * world, game_engine::utility::UniformGrid<int, 2> * grid, ge::GameEngine * engine)
{
    engine_ = engine;
    rng_ = rng;
    grid_ = grid;
    empty_pos_ = empty_pos;
    world_ = world;

    GetInitialPosition();
    SetAsarPath();

    int ret = WorldObject::Init();
    ret = GraphicsObject::Init(position_current_.x, position_current_.y, position_current_.z, "data/cally.cfg", GraphicsObject::ANIMATED);
    //ret = GraphicsObject::Init(position_current_.x, position_current_.y, position_current_.z, "assets/cube.obj");
    world->AddObject(this, position_current_.x, position_current_.y, position_current_.z);

    Scale(0.008f, 0.008f, 0.008f);
    //Scale(0.2f, 0.2f, 0.2f);
    z_rotation_ = ge::math::GetRotateMatrix(ge::math::GetRadians(90), -1, 0, 0);

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
            ResetHard();
            return;
        } else if (c.type_ == COMMAND_CA_PAUSE){
            is_paused_ = !is_paused_;
            return;
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
        
        ResetHard();
        return;
    }

    /* If waypoint is reached, go to the next */
    if (waypoint_ < (path_.size() - 1) && ge::math::Equal(position_current_.z, (float)path_[waypoint_ + 1].i_ + 0.5f, 0.75f) && ge::math::Equal(position_current_.x, (float)path_[waypoint_ + 1].j_ + 0.5f, 0.75f)) {
        waypoint_++;
        last_waypoint_update_ = dt::GetMSecondsSinceEpoch();
    }

    /* Get the desired velocity based on the next waypoint, if target is reached, or if we are stuck, reset */
    bool reached_target = SetDesiredVelocity();
    if (reached_target) {
        Reset();
        return;
    }
    if (last_waypoint_update_ + 10000 < dt::GetMSecondsSinceEpoch()) {
        ResetHard();
        return;
    }

    /* Arrival behaviour, fix maximum speed */
    glm::vec3 target_offset = position_target_ - position_current_;
    float distance = glm::length(target_offset);
    float ramped_speed = max_velocity_norm_ * (distance / 2.8f);
    float clipped_speed = std::min(ramped_speed, max_velocity_norm_);
    glm::vec3 desired_velocity = (clipped_speed) * desired_velocity_direction_;
    float max_arrival_speed = clipped_speed;

    /* Steering behaviour */
    /* Obstacles avoidance */
    /* Get the closest objects in radius of 2.5 */
    std::vector<WorldObject *> neighbors(10);
    ge::math::AABox<2> box(ge::math::Vector2D({ position_current_.x, position_current_.z }), { 5, 5 });
    size_t nof = world_sector_->GetObjectsWindow(box, neighbors);

    ge::Real_t z_forward_closest = 10000;
    for (size_t i = 0; i < nof; i++) {
        if (neighbors[i] == this) continue;
        glm::vec3 other_position = neighbors[i]->GetPosition().ToGlm();
        glm::vec3 my_position = ge::math::Vector3D(position_current_).ToGlm();

        /* Project the obstalces coordinates to our forward and side coordiante system */
        glm::vec3 other_proj = glm::vec3(glm::dot(other_position - my_position, side_), glm::dot(other_position - my_position, up_), glm::dot(other_position - my_position, forward_));
        /* 
            If the z is positive (the obstalce is in the front) 
            the z is smaller than all the others currently found (we want the closest one)
            and the horizontal difference is smaller than 3 radiuses, steer to avoid
        */
        if (other_proj.z > 0 && other_proj.z < z_forward_closest && std::abs(other_proj.x) < radius_ * 3) {
            z_forward_closest = std::min(z_forward_closest, other_proj.z);
            Particle * other = dynamic_cast<Particle *>(neighbors[i]);
            if (other == nullptr) continue;

            GraphicsObject::ExecuteAction();

            /* Set the steering velocity to the other obstacle's side vector */
            steering_velocity_ = -other->side_;
        }
    }

    /* If obstalce avoidance is not activated, do wall avoidance */
    if (!(z_forward_closest < 9999)) {
        glm::vec3 side_position_1 = position_current_ + side_ + 0.5f * forward_;
        glm::vec3 side_position_2 = position_current_ - side_ + 0.5f * forward_;
        if (world_->grid_->at(side_position_1.z, side_position_1.x) == -1) steering_velocity_ = -side_;
        else if (world_->grid_->at(side_position_2.z, side_position_2.x) == -1) steering_velocity_ = side_;
        else {
            /* If no wall is detected, do seek path steering */
            steering_velocity_ = desired_velocity - velocity_current_;
        }
    }

    /* Change the velocity based on the steering velocity */
    velocity_current_ += 2.0f * float(delta_time) * steering_velocity_;
    velocity_current_ = Truncate(velocity_current_, max_arrival_speed);

    glm::vec3 truncated_force = Truncate(force_, max_force_norm_);
    glm::vec3 velocity = velocity_current_;
    /* Update position */
    switch (method_)
    {
    case IntegrationMethod::EulerOrig:
    {
        position_previous_ = position_current_;
        position_current_ += velocity * float(delta_time);
        velocity += ((truncated_force) / mass_) * float(delta_time);
        
        break;
    }
    case IntegrationMethod::EulerSemi:
    {
        position_previous_ = position_current_;
        velocity += ((truncated_force) / mass_) * (float)delta_time;
        position_current_ += velocity * (float)delta_time;
        
        break;
    }
    case IntegrationMethod::Verlet:
    {
        if (ge::math::Equal(position_current_.x, position_previous_.x) && ge::math::Equal(position_current_.y, position_previous_.y) && ge::math::Equal(position_current_.z, position_previous_.z)) {
            velocity += ((truncated_force) / mass_) * (float)delta_time;
            position_current_ += velocity * (float)delta_time;
        } else {
            glm::vec3 temp = position_current_;
            position_current_ = position_current_ + verlet_k_ * (position_current_ - position_previous_) + float(delta_time * delta_time) * ((truncated_force)/ mass_);
            velocity = (position_current_ - temp) / ((float) delta_time);
            position_previous_ = temp;
        }

        break;
    }
    }

    /* Calculate forward and side vectors, our local coordinate system */
    forward_ = glm::normalize(velocity);
    side_ = glm::cross(forward_, up_);
    /* Up does not change */
    //up_ = glm::cross(forward_, side_);
    
    CheckWallCollisions();

    /* Check for collisions with other objects */
    /* For all the objects that we got previously in the obstalce avoidance algorithm */
    for (size_t i = 0; i < nof; i++) {
        if (neighbors[i] == this) continue;
        ge::math::Vector3D other_position = neighbors[i]->GetPosition();
        ge::math::Vector3D my_position = ge::math::Vector3D(position_current_);

        /* If distance is smaller than 2 times the radius */
        if (ge::math::Vector3D::Distance(my_position, other_position) < 2.0f * radius_) {
            /* Calculate collision normal, and change velocity based on the collision */
            ge::math::Vector3D collision_normal = (my_position - other_position).Normalise();
            glm::vec3 corrected = CorrectPlaneCollision(collision_normal.ToGlm(), velocity_current_);
            velocity_current_ = Truncate(velocity_current_ + float(delta_time) * corrected * max_velocity_norm_, max_velocity_norm_);

            /* Check collision independently for horizontal and vertical movement */
            Vector3D corrected_one = Vector3D({ position_current_.x, 0, position_previous_.z });
            Vector3D corrected_two = Vector3D({ position_previous_.x, 0, position_current_.z });
            if (Vector3D::Distance(corrected_one, other_position) >= 2.0f * radius_) {
                position_current_ = corrected_one.ToGlm();
            } else if (Vector3D::Distance(corrected_two, other_position) >= 2.0f * radius_) {
                position_current_ = corrected_two.ToGlm();
            } else {
                position_current_ = position_previous_;
            }
        }

    }

    /* Update positions and animations */
    WorldObject::SetPosition(position_current_[0], position_current_[1], position_current_[2]);
    GraphicsObject::SetRotation(z_rotation_* ge::math::GetRotateMatrix(std::atan2(velocity_current_.x, velocity_current_.z), 0, 0, 1));
    GraphicsObject::UpdateAnimation(delta_time * (glm::length(velocity) / max_velocity_norm_));
    return;
}

void Particle::Draw(game_engine::graphics::Renderer * renderer) {
    renderer->Draw(this);

    /*for (size_t i = waypoint_; i < path_.size() - 1; i++) {
        renderer->DrawLine(ge::math::Vector3D({path_[i].j_ + 0.5f, 0.1, path_[i].i_ + 0.5f}),
            ge::math::Vector3D({ path_[i + 1].j_ + 0.5f, 0.1, path_[i + 1].i_ + 0.5f }), 8, glm::vec3(0, 0, 0));
    }

    renderer->DrawLine(Vector3D(position_current_), Vector3D(position_current_) + Vector3D(forward_), 8, glm::vec3(1, 0, 0));
    renderer->DrawLine(Vector3D(position_current_), Vector3D(position_current_) + Vector3D(side_), 8, glm::vec3(0, 0, 1));*/
}

glm::vec3 Particle::CorrectPlaneCollision(glm::vec3 normal, glm::vec3 velocity) {
    glm::vec3 n = normal;

    glm::vec3 velocity_t = velocity;
    /* Update position, apply elasticity */
    velocity_t = velocity_t - (Real_t(1 + bouncing_)) * (glm::dot(n, velocity_t)) * n;

    /* Update velocity with friction force */
    velocity_t = velocity_t - friction_ * (velocity - glm::dot(n, velocity) * n);
    return velocity_t;
}

void Particle::CheckWallCollisions() {
    /* Calculate our current cell */
    int myi = position_current_.z, myj = position_current_.x;

    /* First check horizontal collision */
    ge::math::Circle2D me1(position_current_.x, position_previous_.z, radius_);
    bool horizontal_intersection = false;
    glm::vec3 normal;
    /* For -1,0,1 neighboring cells, check if they are walls */
    for (int i = -1; i <= 1 && !horizontal_intersection; i++) {
        for (int j = -1; j <= 1 && !horizontal_intersection; j++) {
            int wall_i = myi + i;
            int wall_j = myj + j;
            if (grid_->at(wall_i, wall_j) == -1) {
                /* Compute the intersection between the walls rectangle, and our circle */
                ge::math::Rectangle2D wall(wall_j + 0.5f, wall_i + 0.5f, 1, 1);
                if (IntersectRect_Circle(wall, me1)) {
                    /* If there is collision, set the normal (an approximation) */
                    horizontal_intersection = true;
                    normal = glm::normalize(position_current_ - glm::vec3(wall_j + 0.5f, 0, wall_i + 0.5f));
                }
            }
        }
    }
    /* If collision was detected, set the horizontal position to the previous one, change velocity based on the collision normal */
    if (horizontal_intersection) {
        position_current_.x = position_previous_.x;
        glm::vec3 corrected = CorrectPlaneCollision(normal, velocity_current_);
        velocity_current_ = Truncate(velocity_current_ + float(0.016f) * corrected * max_velocity_norm_, max_velocity_norm_);
    }

    /* Do the same for the vertical movement */
    ge::math::Circle2D me2(position_previous_.x, position_current_.z, radius_);
    bool vertical_intersection = false;
    for (int i = -1; i <= 1 && !vertical_intersection; i++) {
        for (int j = -1; j <= 1 && !vertical_intersection; j++) {
            int wall_i = myi + i;
            int wall_j = myj + j;
            if (grid_->at(wall_i, wall_j) == -1) {
                ge::math::Rectangle2D wall(wall_j + 0.5f, wall_i + 0.5f, 1, 1);
                if (IntersectRect_Circle(wall, me2)) {
                    vertical_intersection = true;
                    normal = glm::normalize(position_current_ - glm::vec3(wall_j + 0.5f, 0, wall_i + 0.5f));
                }
            }
        }
    }
    if (vertical_intersection) {
        position_current_.z = position_previous_.z;
        glm::vec3 corrected = CorrectPlaneCollision(normal, velocity_current_);
        velocity_current_ = Truncate(velocity_current_ + float(0.016f) * corrected * max_velocity_norm_, max_velocity_norm_);
    }

}

glm::vec3 Particle::GetInitialPosition() {
    /* Get random position from the empty ones */
    std::pair<int, int> random_pos = empty_pos_->at(rng_->genrand_real3() * empty_pos_->size());

    position_current_ = glm::vec3(random_pos.second, 0, random_pos.first) + glm::vec3(0.5, 0, 0.5);
    position_previous_ = position_current_;
    velocity_current_ = glm::vec3(0, 0, 0);
    return position_current_;
}

void Particle::Reset() {
    /* Set start position */
    glm::vec3 initial_position = position_current_;

    /* Set A* path */
    SetAsarPath();

    WorldObject::SetPosition(initial_position[0], initial_position[1], initial_position[2]);
    lifetime_ = (immortal_) ? std::numeric_limits<Real_t>::max() : rng_->genrand_real3() * 15;
}

void Particle::ResetHard() {
    /* Set start position */
    glm::vec3 initial_position = GetInitialPosition();

    /* Set A* path */
    SetAsarPath();

    WorldObject::SetPosition(initial_position[0], initial_position[1], initial_position[2]);
    lifetime_ = (immortal_) ? std::numeric_limits<Real_t>::max() : rng_->genrand_real3() * 15;

    GraphicsObject::StopAction();
}

void Particle::SetAsarPath() {
    /* Get random position from the empty ones */
    std::pair<int, int> random_goal = empty_pos_->at(rng_->genrand_real3() * empty_pos_->size());
    position_target_ = glm::vec3(random_goal.second, 0, random_goal.first) + glm::vec3(0.5, 0, 0.5);

    /* Run A* */
    astar.Init(grid_, BidirectionalAstar::CELL(position_current_.z, position_current_.x), BidirectionalAstar::CELL(random_goal.first, random_goal.second));
    astar.FullRunBI();
    path_ = astar.GetPath();
    
    waypoint_ = 0;
    last_waypoint_update_ = dt::GetMSecondsSinceEpoch();
    
    SetDesiredVelocity();
}

bool Particle::SetDesiredVelocity() {
    /* If path exists, set the desired velocity to be from current position to next path waypoint, otherwise its zero */
    if (waypoint_ + 1 < path_.size()) {
        desired_velocity_direction_ = glm::vec3(path_[waypoint_ + 1].j_ + 0.5f - position_current_.x,
            0,
            path_[waypoint_ + 1].i_ + 0.5f - position_current_.z);

        desired_velocity_direction_ = glm::normalize(desired_velocity_direction_);

        //desired_velocity_ = max_velocity_norm_ * glm::normalize(desired_velocity_);
        return false;
    } else {
        desired_velocity_direction_ = glm::vec3(0);
        return true;
    }
}

glm::vec3 Particle::Truncate(glm::vec3 & input, float max_length) {
    float input_l = glm::length(input);
    if (input_l > max_length) {
        glm::vec3 temp = (input / input_l) * max_length;
        return temp;
    }
    else
        return input;
}

int Particle::Destroy() {

    return 0;
}

void Particle::SetIntegrationMethod(IntegrationMethod m) {
    method_ = m;
}

void Particle::SetPositionCurrent(glm::vec3 p) {
    position_current_ = p;
}

void Particle::SetPositionPrevious(glm::vec3 p) {
    position_previous_ = p;
}

void Particle::SetVelocity(glm::vec3 p) {
    velocity_current_ = p;
    position_previous_ = position_current_ - 0.016f * velocity_current_;
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
    return velocity_current_;
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

void Particle::Set1() {
    std::pair<int, int> random_pos = { 2,2 };

    position_current_ = glm::vec3(random_pos.second, 0, random_pos.first) + glm::vec3(0.5, 0, 0.5);
    position_previous_ = position_current_;
    velocity_current_ = glm::vec3(0, 0, 0);

    std::pair<int, int> random_goal = { 2, 10 };
    position_target_ = glm::vec3(random_goal.second, 0, random_goal.first) + glm::vec3(0.5, 0, 0.5);

    astar.Init(grid_, BidirectionalAstar::CELL(position_current_.z, position_current_.x), BidirectionalAstar::CELL(random_goal.first, random_goal.second));
    astar.FullRunBI();
    path_ = astar.GetPath();
    waypoint_ = 0;
    SetDesiredVelocity();

    WorldObject::SetPosition(position_current_[0], position_current_[1], position_current_[2]);
}

void Particle::Set2() {
    std::pair<int, int> random_pos = { 2.8, 10 };

    position_current_ = glm::vec3(random_pos.second, 0, random_pos.first) + glm::vec3(0.5, 0, 0.5);
    position_previous_ = position_current_;
    velocity_current_ = glm::vec3(0, 0, 0);

    std::pair<int, int> random_goal = { 2.8, 2 };
    position_target_ = glm::vec3(random_goal.second, 0, random_goal.first) + glm::vec3(0.5, 0, 0.5);
    astar.Init(grid_, BidirectionalAstar::CELL(position_current_.z, position_current_.x), BidirectionalAstar::CELL(random_goal.first, random_goal.second));
    astar.FullRunBI();
    path_ = astar.GetPath();
    waypoint_ = 0;
    SetDesiredVelocity();

    WorldObject::SetPosition(position_current_[0], position_current_[1], position_current_[2]);
}
