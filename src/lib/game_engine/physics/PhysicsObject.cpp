#include "PhysicsObject.hpp"
#include "PhysicsEngine.hpp"

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace game_engine {

namespace physics {

    PhysicsObject::PhysicsObject() {
        

        pos_x_ = 0.0f;
        pos_y_ = 0.0f;
        pos_z_ = 0.0f;

        is_inited_ = false;
    }

    int PhysicsObject::Init(float pos_x, float pos_y, float pos_z, PhysicsEngine * engine) {
        
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

    float PhysicsObject::GetX() {
        return pos_x_;
    }

    float PhysicsObject::GetY() {
        return pos_y_;
    }

    float PhysicsObject::GetZ() {
        return pos_z_;
    }

    void PhysicsObject::SetPosition(float pos_x, float pos_y, float pos_z) {
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

    void PhysicsObject::SetCollision(float x_size, float y_size, float z_size) {
        delete collision_;
        collision_ = new CollisionBoundingRectangle(Rectangle2D(pos_x_, pos_y_, x_size, y_size));
    }

    void PhysicsObject::SetCollision(float radius) {
        delete collision_;
        collision_ = new CollisionBoundingCircle(Circle2D(pos_x_, pos_y_, radius));
    }

    CollisionType PhysicsObject::GetCollisionType() {
        return collision_->GetType();
    }

    Collision * PhysicsObject::GetCollision() {
        return collision_;
    }

    CollisionResult_t PhysicsObject::CheckCollision(float move_offset, Direction direction) {
        return physics_engine_->CheckCollision(this, move_offset, direction);
    }

    void PhysicsObject::OnCollisionDetected(size_t object_type) {
        
    }

    void PhysicsObject::Rotate(float angle) {
        collision_->Rotate(angle);
    }

    size_t PhysicsObject::GetObjectType() {
        return object_type_;
    }

    void PhysicsObject::SetObjectType(size_t type) {
        object_type_ = type;
    }

    bool PhysicsObject::Collides(Point2D new_position, PhysicsObject * other) {

        if (!is_inited_) return false;

        /* Apply rotations if applicable */
        CollisionType neighbour_collision_type = other->GetCollisionType();
        CollisionType moving_object_collision_type = collision_->GetType();

        /* We do not collide with ourselves */
        if (other == this) return false;

        if (neighbour_collision_type == CollisionType::COLLISION_NONE) return false;
        if (moving_object_collision_type == CollisionType::COLLISION_NONE) return false;

        /* Move the collision object to new position */
        float x_offset = new_position.x_ - pos_x_;
        float y_offset = new_position.y_ - pos_y_;
        collision_->Translate(x_offset, y_offset);
        Collision * neighbour_collision = other->GetCollision();

        /* Check collision */
        bool collides = neighbour_collision->Check(collision_);

        if (collides) {
            this->OnCollisionDetected(other->GetObjectType());
            other->OnCollisionDetected(object_type_);
        }

        /* Move collision object back */
        collision_->Translate(-x_offset, -y_offset);

        return collides;
    }

}

}