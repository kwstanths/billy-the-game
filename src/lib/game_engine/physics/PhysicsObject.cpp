#include "PhysicsObject.hpp"
#include "PhysicsEngine.hpp"

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace ge = game_engine;
namespace math = game_engine::math;

namespace game_engine {
namespace physics {

    PhysicsObject::PhysicsObject() {
        
        removable_ = false;
        pos_x_ = 0.0f;
        pos_y_ = 0.0f;
        pos_z_ = 0.0f;

        is_inited_ = false;
    }

    int PhysicsObject::Init(ge::Real_t pos_x, ge::Real_t pos_y, ge::Real_t pos_z, PhysicsEngine * engine) {
        
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* Set the initial collision to none */
        SetCollision();

        /* Set position */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

        /* Insert into the world */
        physics_engine_ = engine;
        int ret = physics_engine_->Insert(this);
        if (ret) return ret;

        is_inited_ = true;
        return 0;
    }

    int PhysicsObject::Destroy() {

        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        physics_engine_->Remove(this);
        delete collision_;

        is_inited_ = false;
        return 0;
    }

    bool PhysicsObject::IsInited() {
        return is_inited_;
    }

    ge::Real_t PhysicsObject::GetX() {
        return pos_x_;
    }

    ge::Real_t PhysicsObject::GetY() {
        return pos_y_;
    }

    ge::Real_t PhysicsObject::GetZ() {
        return pos_z_;
    }

    void PhysicsObject::SetPosition(ge::Real_t pos_x, ge::Real_t pos_y, ge::Real_t pos_z) {
        /* Update the collision detection objects */
        collision_->Translate(pos_x - pos_x_, pos_y - pos_y_);
        
        /* Update the object's position inside the physics engine */
        physics_engine_->Update(this, pos_x, pos_y);

        /* Set the internal parameeters */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;
    }

    void PhysicsObject::SetCollision() {
        delete collision_;
        collision_ = new CollisionNone();
    }

    void PhysicsObject::SetCollision(math::Rectangle2D rect) {
        delete collision_;
        collision_ = new CollisionBoundingRectangle(rect);
    }

    void PhysicsObject::SetCollision(math::Circle2D circle) {
        delete collision_;
        collision_ = new CollisionBoundingCircle(circle);
    }

    CollisionType PhysicsObject::GetCollisionType() {
        return collision_->GetType();
    }

    Collision * PhysicsObject::GetCollision() {
        return collision_;
    }

    void PhysicsObject::Rotate(ge::Real_t angle) {
        collision_->Rotate(angle);
    }

    size_t PhysicsObject::GetObjectType() {
        return object_type_;
    }

    void PhysicsObject::SetObjectType(size_t type) {
        object_type_ = type;
    }

    void PhysicsObject::OnCollisionDetected(size_t type) {

    }

    bool PhysicsObject::Collides(math::Point2D new_position, PhysicsObject * other) {

        if (!is_inited_) return false;

        /* Apply rotations if applicable */
        CollisionType neighbour_collision_type = other->GetCollisionType();
        CollisionType moving_object_collision_type = collision_->GetType();

        /* We do not collide with ourselves */
        if (other == this) return false;

        if (neighbour_collision_type == CollisionType::COLLISION_NONE) return false;
        if (moving_object_collision_type == CollisionType::COLLISION_NONE) return false;

        /* Move the collision object to new position */
        ge::Real_t x_offset = new_position.x_ - pos_x_;
        ge::Real_t y_offset = new_position.y_ - pos_y_;
        collision_->Translate(x_offset, y_offset);
        Collision * neighbour_collision = other->GetCollision();

        /* Check collision */
        bool collides = neighbour_collision->Check(collision_);

        if (collides) {
            /* Do something here TODO */
        }

        /* Move collision object back */
        collision_->Translate(-x_offset, -y_offset);

        return collides;
    }

}

}