#ifndef __PhysicsObject_hpp__
#define __PhysicsObject_hpp__

#include "game_engine/math/Types.hpp"
#include "Collision.hpp"

namespace game_engine {
namespace physics {

    class PhysicsEngine;

    /**
        A physics object
    */
    class PhysicsObject {
        friend PhysicsEngine;
    public:
        /**
            Does nothing in particular. Call Init()
        */
        PhysicsObject();

        virtual ~PhysicsObject() {};

        /**
            Initializes the physics object
            @param pos_x The position x
            @param pos_y The position y
            @param pos_z The position z
            @param engine The physics engine to use
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(game_engine::Real_t pos_x, game_engine::Real_t pos_y, game_engine::Real_t pos_z, PhysicsEngine * engine);

        /**
            Removes from the pgysics engine, TODO
            @return 0=OK
        */
        int Destroy();

        /**
            Get whether the object is initialised or not
            @return true = initialised, false = not initialised
        */
        bool IsInited();

        /**
            Get the position x coordinate
            @return position x coordinate
        */
        game_engine::Real_t GetX();

        /**
            Get the position y coordinate
            @return position y coordinate
        */
        game_engine::Real_t GetY();

        /**
            Get the position z coordinate
            @return position z coordinate
        */
        game_engine::Real_t GetZ();

        /**
            Set the position of the object
            @param pos_x Position x coordinate
            @param pos_y Position y coordinate
            @param pos_z Position z coordinate
        */
        void SetPosition(game_engine::Real_t pos_x, game_engine::Real_t pos_y, game_engine::Real_t pos_z);
    
        /**
            Set collision to none
        */
        void SetCollision();

        /**
        
        */
        void SetCollision(game_engine::math::Rectangle2D rect);

        /**
        
        */
        void SetCollision(game_engine::math::Circle2D circle);

        /**
            Get the type of collision
            @return The collision type
        */
        CollisionType GetCollisionType();

        /**
            Get the collision object
            @return The collision object
        */
        Collision * GetCollision();

        /**
            Rotate the physics object clockwise in the 2d pane
            @angle angle The rotation angle in radians
            @param axis The axis to rotate around
        */
        void Rotate(game_engine::Real_t angle);

        /**
        
        */
        size_t GetObjectType();

        /**
        
        */
        void SetObjectType(size_t type);

        /**
        
        */
        virtual void OnCollisionDetected(size_t type);

    protected:
        bool removable_;

    private:
        bool is_inited_;
        game_engine::Real_t pos_x_, pos_y_, pos_z_;
        size_t object_type_ = 0;

        Collision * collision_ = nullptr;
        PhysicsEngine * physics_engine_;

        /**
            Get wether this object collides with another object in a new position
            @param new_position The new position of the object
            @param other The other object
            @return true = collides, false =  not collides
        */
        bool Collides(game_engine::math::Point2D new_position, PhysicsObject * other);
    };

}

}

#endif