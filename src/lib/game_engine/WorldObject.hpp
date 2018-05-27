#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include <functional>
#include <string>
#include <exception>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "physics/Types.hpp"
#include "memory/ArrayAllocator.hpp"
#include "memory/PoolAllocator.hpp"
#include "debug_tools/Console.hpp"

#include "Collision.hpp"   

#include "glm/glm.hpp"

namespace game_engine {

    class WorldSector;

    /**
        A WorldObject is an entity inside a WorldSector. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() function
    */
    class WorldObject {
        friend WorldSector;
    public:

        /**
            Does nothing in particular. Call Init()
        */
        WorldObject();

        static void * operator new(size_t size, memory_subsystem::ArrayAllocator * array_objects) {
            void * address = array_objects->Allocate(size);
            if (address == nullptr) {

                debug_tools::ConsoleInfoL(debug_tools::FATAL,
                    "ArrayAllocator::Allocate(size): size does not fit inside the memory page used",
                    "Requested size: ", size);
                
                throw std::bad_alloc();
            }
            return address;
        }

        static void * operator new(size_t size, memory_subsystem::PoolAllocator * pool_objects) {
            
            if (pool_objects->GetBlockSize() < size) throw std::bad_alloc();

            void * address = pool_objects->Allocate();
            if (address == nullptr) throw std::bad_alloc();

            return address;
        }

        static void operator delete(void * ptr, memory_subsystem::ArrayAllocator * array_objects) {
            debug_tools::Console(debug_tools::FATAL, "WorldObject array allocation failed");
        }

        static void operator delete(void * ptr, memory_subsystem::PoolAllocator * pool_objects) {
            debug_tools::Console(debug_tools::FATAL, "WorldObject pool allocation failed");
        }

        /**
            Sets the OpenGL model and texture to the object. Also sets the renderer to be used
            Initial position is at (0,0,0), scaling is at (1,1,1) and rotation at 0 degrees
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(OpenGLObject * object, OpenGLTexture * texture);

        /**
            Does nothing in particular
        */
        int Destroy();

        /**
            Get whether or not the object is initialised
        */
        bool IsInited();

        /**
            Draw the object using the variables set in Init(...);
        */
        void Draw(OpenGLRenderer * renderer);

        /**
            Function that should be overriden
        */
        virtual void Step(double delta_time);

        /**
            Get the x coordinate of the object's position
            @return The x coordinate
        */
        float GetXPosition();

        /**
            Get the y coordinate of the object's position
            @return The y coordinate
        */
        float GetYPosition();

        /**
            Get the z coordinate of the object's position
            @return The z coordinate
        */
        float GetZPosition();

        /**
            Set the position for the object, sets translation matrix
            @param pos_x Position x coordinate
            @param pos_y Position y coordinate
            @param pos_z Position z coordinate
        */
        void SetPosition(float pos_x, float pos_y, float pos_z);

        /**
            Scale the object, sets the scale matrix
            @param Scale amount in axis x
            @param Scale amount in axis y
            @param Scale amount in axis z
        */
        void Scale(float scale_x, float scale_y, float scale_z);

        /**
            Rotate the object
            @param angle The angle of rotation, Can be negative
            @param axis The axis to rotate arounf, 0=X-axis, 1=Y-axis, 2=Z-axis
        */
        void Rotate(float angle, size_t axis);

        /**
            Sets collision to NONE
        */
        void SetCollision();
        
        /**
            Sets collision to Bounding rectangle
            @param x_size Rectangle horizontal width
            @param y_size Rectangle vertical width
        */
        void SetCollision(float x_size, float y_size, float z_size = 0.0f);

        /**
            Sets collision to Bounding circle
            @param radius The circle radius
        */
        void SetCollision(float radius);

        /**
            Get the type of collision this object has set
        */
        CollisionType GetCollisionType();

        /**
        
        */
        Collision * GetCollision();

        /**
            Check if this object in new_position collides with another object
            @param new_position The new position
            @param other The other object
        */
        bool Collides(Point2D new_position, WorldObject * other);

    private:
        bool is_inited_;
        float pos_x_, pos_y_, pos_z_;
        float rotated_angle_;
        Collision * collision_;


        OpenGLObject * object_;
        OpenGLTexture * texture_;
        CollisionType collision_type_;
        WorldSector * world_sector_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_;
    };

}
#endif