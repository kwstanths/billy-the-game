#ifndef __WorldSector_hpp__
#define __WorldSector_hpp__

#include <vector>
#include <deque>

#include "game_engine/memory/MemoryManager.hpp"
#include "game_engine/physics/PhysicsEngine.hpp"
#include "game_engine/utility/CircularBuffer.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"

#include "WorldObject.hpp"


namespace game_engine {
    
    /**
        A class to store world objects
    */
    class WorldSector {
        friend class GameEngine;
    public:
        /**
            Does nothing in particular. Call Init()
        */
        WorldSector();

        /**
            Initializes the internal structures used to hold the world objects
            @param width The number of columns to be used in the 2d world
            @param height The number of rows to be used in the 2d world
            @param x_margin_start The outermost left horizontal coordinate
            @param x_margin_end The outermost right horizontal coordinate
            @param y_margin_start The outermost down vertical coordinate
            @param y_margin_end The outermost top vertical coordinate 
            @param elements The maximum number of elements to hold
            @return 0=OK, -1 = Not initialised
        */
        int Init(size_t width, size_t height, 
            Real_t x_margin_start, Real_t x_margin_end,
            Real_t y_margin_start, Real_t y_margin_end, 
            size_t elements);

        /**
            Clears the world, DOES NOT destroy the items themselves
            @return true = OK
        */
        int Destroy();

        /**
            Check whether the object is initialised
            @return Is initialised
        */
        bool IsInited();

        /**
            Spawn a new object. Allocates the memory, and returns a pointer to it
            @param removable If true, when the object is removed from the world no memory leak will occur
            @return A pointer to the object. If nullptr, then allocation failed
        */
        template<typename T> T * NewObj(bool removable = false) {
            if (!is_inited_) {
                dt::Console(dt::CRITICAL, "World sector is not initialised");
                return nullptr;
            }

            memory::MemoryManager & memory_manager = memory::MemoryManager::GetInstance();

            T * the_new_object;
            try{
            
                /* Allocate memory for new object */
                if (!removable) the_new_object = new (memory_manager.GetStaticObjectsAllocator()) T();
                else the_new_object = new (memory_manager.GetRemovableObjectsAllocator()) T();
            
            } catch (...) {
                dt::Console(dt::FATAL, "Memory allocation failed");
                return nullptr;
            }

            the_new_object->world_sector_ = this;
            the_new_object->removable_ = removable;
            
            return the_new_object;
        }

        void Step(math::Point2D camera_center, Real_t camera_width, double delta_time, graphics::Renderer * renderer);

        /**
            Inserts a new object to the world. 
            @param object A pointer to the object to insert
            @param x Position X coordinate
            @param y Position Y coordinate
            @param z Position Z coordinate
            @return 0 = OK, -1 = Not initialised
        */
        int Insert(WorldObject * object, Real_t x, Real_t y, Real_t z);

        /**
            Updates the position of an object in the world
            @param object The object
            @param old_pos_x The old position x coordinate
            @param old_pos_y The old position y coordinate
            @param new_pos_x The new position x coordinate
            @param new_pos_y The new position y coordinate
        */
        void UpdateObjectPosition(WorldObject * object, Real_t old_pos_x, Real_t old_pos_y, Real_t new_pos_x, Real_t new_pos_y);

        /**
            Remove the object from the world
            @param object Object to remove
        */
        void Remove(WorldObject * object);

        /**
            Get a window of object in the world. Objects are assigned sequentially to the visible world 
            vector. Does not allocate any new memory
            @param center_x The x coordinate of the window's center
            @param center_y The y coordinate of the window's center
            @param margin The top, down, left, right window margin
            @param[out] visible_world The vector with pointers to the objects window
            @return The number of objects assigned to the visible_world vector
        */
        size_t GetObjectsWindow(Real_t center_x, Real_t center_y, Real_t margin, std::vector<WorldObject *> & objects);

        /**
            Find  the first interactable object inside the search area. pos_x, pos_y is currently not used
            @param search_area The area to search for objects
            @param pos_x currently not used
            @param pos_y currently not used
            @return nullptr if none is found, the pointer if something is found
        */
        WorldObject * FindInteractNeighbour(game_engine::math::Rectangle2D search_area, math::Point2D p, Real_t size);

        /**
            Get the physics engine used in this world
            @return The physics engine. nullptr if not initialised
        */
        physics::PhysicsEngine * GetPhysicsEngine();

    private:
        bool is_inited_;
        Real_t x_margin_start_, x_margin_end_, y_margin_start_, y_margin_end_;

        /* Each world holds an instance of a physics engine */
        physics::PhysicsEngine * physics_engine_;
        
        /* 
            A struct that resembles the world. Holds pointers to actual objects that are stored 
            sequentially
        */
        std::vector<std::vector<std::deque<WorldObject *> > >world_;
        std::vector<WorldObject *> visible_world_;

        /* Holds objects that are removed from the world, whose memory needs deallocation */
        utility::CircularBuffer<WorldObject *> delete_objects_buffer_;

        /**
            Get the row in the world based in the vertical coordiate
            @param vertical_coordinate The vertical coordinate
            @return The row
        */    
        size_t GetRow(Real_t vertical_coordinate);
        
        /**
            Get the column in the world based in the horizontal coordinate
            @param horizontal_coordiante The horizontal coordiante
            @return The column
        */
        size_t GetColumn(Real_t horizontal_coordinate);

        /**
            Deallocate the removed objects
        */
        void DeleteRemovedObjects();

    };

}

#endif
