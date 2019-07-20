#include "PhysicsObject.hpp"

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

    int PhysicsObject::Init(ge::Real_t pos_x, ge::Real_t pos_y, ge::Real_t pos_z) {
        
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        /* Set position */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

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
        
        /* Set the internal parameeters */
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;
    }

}

}