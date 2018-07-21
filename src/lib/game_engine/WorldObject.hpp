#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include <functional>
#include <string>
#include <exception>

#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/graphics/GraphicsObject.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/physics/PhysicsObject.hpp"
#include "game_engine/memory/ArrayAllocator.hpp"
#include "game_engine/memory/PoolAllocator.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {

    class WorldSector;

    /**
        A WorldObject is an entity inside a WorldSector. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() and Interact function
    */
    class WorldObject : public physics::PhysicsObject, public graphics::GraphicsObject {
        friend WorldSector;
    public:

        /**
            Does nothing in particular. Call Init()
        */
        WorldObject();

        /**
            Custom sequential alloction without single remove and delete
        */
        void * operator new(size_t size, memory_subsystem::ArrayAllocator * array_objects) {
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
        void * operator new(size_t size, memory_subsystem::PoolAllocator * pool_objects) {
            
            if (pool_objects->GetBlockSize() < size) throw std::bad_alloc();

            void * address = pool_objects->Allocate();
            if (address == nullptr) throw std::bad_alloc();

            return address;
        }

        void operator delete(void * ptr, memory_subsystem::ArrayAllocator * array_objects) {
            debug_tools::Console(debug_tools::FATAL, "WorldObject array allocation failed");
        }

        void operator delete(void * ptr, memory_subsystem::PoolAllocator * pool_objects) {
            debug_tools::Console(debug_tools::FATAL, "WorldObject pool allocation failed");
        }

        /**
            Initialize the object. Sets the OpenGL model and texture of the object. Sets the initial position
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(std::string model_file_path, Real_t x, Real_t y, Real_t z, bool interactable = false);

        /**
            Does nothing in particular yet
        */
        int Destroy();

        /**
            Get whether or not the object is initialised
        */
        bool IsInited();

        /**
            Draw the object during every frame, after calling the step function. Override for custom behaviour
            If not overriden, the object is drawn with full lightning
            @param renderer The renderer
        */
        virtual void Draw(graphics::Renderer * renderer);

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
        void SetPosition(Real_t pos_x, Real_t pos_y, Real_t pos_z, bool collision_check = true);

        /**
            Scale the object, sets the scale matrix. Collision detection is NOT changed TODO
            @param Scale amount in axis x
            @param Scale amount in axis y
            @param Scale amount in axis z
        */
        void Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z);

        /**
            Rotate the object around an axis. Changes collision detection as well
            @param angle The angle of rotation in radians, can be negative
            @param axis The axis to rotate around
        */
        void Rotate(Real_t angle, glm::vec3 axis);

    protected:
        WorldSector * world_sector_ = nullptr;

    private:
        bool is_inited_;
        bool removable_;
        bool interactable_;

    };

}
#endif