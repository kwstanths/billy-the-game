#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include <functional>
#include <string>
#include <exception>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "physics/Types.hpp"
#include "physics/PhysicsObject.hpp"
#include "memory/ArrayAllocator.hpp"
#include "memory/PoolAllocator.hpp"
#include "debug_tools/Console.hpp"

#include "glm/glm.hpp"

namespace game_engine {

    class WorldSector;

    /**
        A WorldObject is an entity inside a WorldSector. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() and Interact function
    */
    class WorldObject : public physics::PhysicsObject {
        friend WorldSector;
    public:

        /**
            Does nothing in particular. Call Init()
        */
        WorldObject();

        /**
            Custom sequential alloction without single remove and delete
        */
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

        /**
            Custom sequential allocation without remove and delete
        */
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
            Initialize the object. Sets the OpenGL model and texture of the object. Sets the initial position
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(OpenGLObject * object, OpenGLTexture * texture, float x, float y, float z);

        /**
            Does nothing in particular yet
        */
        int Destroy();

        /**
            Get whether or not the object is initialised
        */
        bool IsInited();

        /**
            Draw the object during every frame, after calling the step function
            @param renderer The renderer
        */
        void Draw(OpenGLRenderer * renderer);

        /**
            Function that should be overriden for custom step behaviour. Step() is called,
            and then Draw() is called
        */
        virtual void Step(double delta_time);

        /**
            Function that can be overriden for custom interaction. It's not called during 
            frames. Call it yourself for another object
        */
        virtual void Interact();

        /**
            Set the position for the object
            @param pos_x Position x coordinate
            @param pos_y Position y coordinate
            @param pos_z Position z coordinate
        */
        void SetPosition(float pos_x, float pos_y, float pos_z);

        /**
            Scale the object, sets the scale matrix. Collision detection is NOT changed TODO
            @param Scale amount in axis x
            @param Scale amount in axis y
            @param Scale amount in axis z
        */
        void Scale(float scale_x, float scale_y, float scale_z);

        /**
            Rotate the object. Changes collision detection as well
            @param angle The angle of rotation in radians, can be negative
            @param axis The axis to rotate around, 0=X-axis, 1=Y-axis, 2=Z-axis
        */
        void Rotate(float angle, size_t axis);

        /**
            Get the diercton at which the object is "looking". The initial direction is "top" 
            which is zero degrees angle
            @return The direction of the object in RADIANS
        */
        Direction GetLookingDirection();

    protected:
        WorldSector * world_sector_ = nullptr;

    private:
        bool is_inited_;
        float rotated_angle_;

        OpenGLObject * object_;
        OpenGLTexture * texture_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_;
    };

}
#endif