#ifndef __PhysicsObject_hpp__
#define __PhysicsObject_hpp__

#include "Types.hpp"
#include "game_engine/Collision.hpp"

namespace game_engine {

namespace physics {

    class PhysicsEngine;

    class PhysicsObject {
    public:
        PhysicsObject();

        int Init(float pos_x, float pos_y, float pos_z, PhysicsEngine * engine_);

        int Destroy();

        bool IsInited();

        float GetX();

        float GetY();

        float GetZ();

        void SetPosition(float pos_x, float pos_y, float pos_z);
    
        void SetCollision();

        void SetCollision(float x_size, float y_size, float z_size = 0.0f);

        void SetCollision(float radius);

        CollisionType GetCollisionType();

        Collision * GetCollision();

        CollisionResult_t CheckCollision(float move_offset, Direction direction);

        bool Collides(Point2D new_position, PhysicsObject * other);

    private:
        bool is_inited_;
        float pos_x_, pos_y_, pos_z_;
        
        Collision * collision_;
        PhysicsEngine * physics_engine_;
    };

}

}

#endif