#ifndef __WorldSector_hpp__
#define __WorldSector_hpp__

#include <vector>
#include <deque>

#include "game_engine/memory/MemoryManager.hpp"
#include "game_engine/utility/CircularBuffer.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vec3.hpp"
#include "game_engine/utility/QuadTree.hpp"
#include "game_engine/utility/UniformGrid.hpp"

#include "WorldObject.hpp"


namespace game_engine {
    
    /**
        A class to store world objects
    */
    class WorldSector {
        friend class GameEngine;
        friend WorldObject;
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

            the_new_object->removable_ = removable;
            
            return the_new_object;
        }

        /**
            
        */
        void Step(double delta_time, graphics::Renderer * renderer, math::Vec3 camera_position, math::Vec3 camera_direction, Real_t camera_ratio, Real_t camera_angle);

        /**
            vector.push_back comment
        */
        int AddObject(WorldObject * object, Real_t x, Real_t y, Real_t z, bool is_npc = false);

        /**
        
        */
        int RemoveObject(WorldObject * object);

        /**
        
        */
        int AddLight(graphics::PointLight_t * light, math::Point2D& point);

        /**
        
        */
        int RemoveLight(graphics::PointLight_t * light, math::Point2D& point);

        /**
            Get a window of object in the world. Objects are assigned sequentially to the visible world 
            vector. Does not allocate any new memory
            @param rect The rectangle-window to get the objects. The MUST be in Axis aligned - ABCD correct form
            @param[out] visible_world The vector with pointers to the objects window
            @return The number of objects assigned to the visible_world vector
        */
        size_t GetObjectsWindow(math::Rectangle2D rect, std::vector<WorldObject *> & objects);

        /**
            Find  the first interactable object inside the search area. pos_x, pos_y is currently not used
            @param search_area The area to search for objects
            @param pos_x currently not used
            @param pos_y currently not used
            @return nullptr if none is found, the pointer if something is found
        */
        WorldObject * FindInteractNeighbour(game_engine::math::Rectangle2D search_area, math::Point2D p, Real_t size);

    private:
        bool is_inited_;
        Real_t x_margin_start_, x_margin_end_, y_margin_start_, y_margin_end_;

        /* 
            A struct that resembles the world. Holds pointers to actual objects that are stored 
            sequentially
        */
        utility::UniformGrid<std::deque<WorldObject *>, 2> * world_;
        size_t grid_rows_, grid_columns_;

        std::vector<WorldObject *> visible_world_;
        std::vector<WorldObject *> npcs_;
        
        utility::QuadTree<graphics::PointLight_t *> world_point_lights_;

        /* Holds objects that are removed from the world, whose memory needs deallocation */
        utility::CircularBuffer<WorldObject *> delete_objects_buffer_;

        /**
            Get the row in the world based in the vertical coordiate
            @param vertical_coordinate The vertical coordinate
            @return The row
        */    
        int GetRow(Real_t y_coordinate);
        
        /**
            Get the column in the world based in the horizontal coordinate
            @param horizontal_coordiante The horizontal coordiante
            @return The column
        */
        int GetColumn(Real_t x_coordinate);

        /**
            Inserts a new object to the world.
            @param object A pointer to the object to insert
            @param x Position X coordinate
            @param y Position Y coordinate
            @param z Position Z coordinate
            @return 0 = OK, -1 = Not initialised
        */
        int InsertObjectToWorldStructure(WorldObject * object, Real_t x, Real_t y, Real_t z);


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
        void RemoveObjectFromWorldStructure(WorldObject * object);

        /**

        */
        void DeleteObj(WorldObject * object);

        /**
            Deallocate the removed objects
        */
        void FlushObjectDelete();

    };

}

#endif
