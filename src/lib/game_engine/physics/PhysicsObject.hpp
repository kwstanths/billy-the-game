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
        int Init(float pos_x, float pos_y, float pos_z, PhysicsEngine * engine);

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
        float GetX();

        /**
            Get the position y coordinate
            @return position y coordinate
        */
        float GetY();

        /**
            Get the position z coordinate
            @return position z coordinate
        */
        float GetZ();

        /**
            Set the position of the object
            @param pos_x Position x coordinate
            @param pos_y Position y coordinate
            @param pos_z Position z coordinate
        */
        void SetPosition(float pos_x, float pos_y, float pos_z);
    
        /**
            Set collision to none
        */
        void SetCollision();

        /**
            Set collision to bounding rectangle
            @param x_size The x size
            @param y_size The y size
            @param z_size The z size. <-- This is not used
        */
        void SetCollision(float x_size, float y_size, float z_size = 0.0f);

        /**
            Set the collision to bounding circle
            @param radius The radius of the circle
        */
        void SetCollision(float radius);

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
            Check this object for collision
            @param move_offset The amount of moving
            @param direction The direction of moving
            @return The collision result
        */
        CollisionResult_t CheckCollision(float move_offset, Direction direction);

        /**
            Get wether this object collides with another object in a new position
            @param new_position The new position of the object 
            @param other The other object
            @return true = collides, false =  not collides
        */
        bool Collides(Point2D new_position, PhysicsObject * other);

        /**
            Rotate the physics object clockwise in the 2d pane
            @angle angle The rotation angle in radians
            @param axis The axis to rotate around
        */
        void Rotate(float angle);

    private:
        bool is_inited_;
        float pos_x_, pos_y_, pos_z_;
        
        Collision * collision_ = nullptr;
        PhysicsEngine * physics_engine_;
    };

}

}

#endif