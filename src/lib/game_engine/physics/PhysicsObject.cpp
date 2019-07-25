#include "PhysicsObject.hpp"
#include "PhysicsEngine.hpp"

#include "game_engine/ErrorCodes.hpp"
#include "game_engine/math/AABox.hpp"

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

    int PhysicsObject::Init(ge::Real_t pos_x, ge::Real_t pos_y, ge::Real_t pos_z) {
        
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        /* Set position */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

        collision_ = new CollisionNone();

        is_inited_ = true;
        return 0;
    }

    int PhysicsObject::Destroy() {

        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

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
        
        collision_->Translate(pos_x - pos_x_, pos_y - pos_y_);

        /* Set the internal parameeters */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;
    }

    void PhysicsObject::SetCollision(physics::PhysicsEngine * engine, game_engine::math::AABox<2> box) {
        engine->Insert(this, Point2D({ pos_x_, pos_y_ }));
        collision_ = new CollisionBoundingBox(box);
    }

    bool PhysicsObject::Collides(game_engine::math::Point2D new_position, PhysicsObject * other) {
        if (!is_inited_) return false;

        /* We do not collide with ourselves */
        if (other == this) return false;

        /* Move the collision object to new position */
        ge::Real_t x_offset = new_position.x() - pos_x_;
        ge::Real_t y_offset = new_position.y() - pos_y_;
        collision_->Translate(x_offset, y_offset);
        Collision * neighbour_collision = other->collision_;

        /* Check collision */
        bool collides = neighbour_collision->Check(collision_);

        /* Move collision object back */
        collision_->Translate(-x_offset, -y_offset);

        return collides;
    }

}

}