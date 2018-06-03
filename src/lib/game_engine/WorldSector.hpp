#ifndef __WorldSector_hpp__
#define __WorldSector_hpp__

#include <vector>
#include <deque>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"
#include "physics/Types.hpp"
#include "physics/PhysicsEngine.hpp"
#include "memory/ArrayAllocator.hpp"

#include "GameEngine.hpp"
#include "WorldObject.hpp"


namespace game_engine {
    
    /**
        A class to store world objects in a 2d manner
    */
    class WorldSector {
    public:
        /**
            Does nothing in particular
        */
        WorldSector();

        /**
            Initializes the internal structures used to hold the world objects
            @param width The number of columns to be used in the 2d world
        */
        int Init(size_t width, size_t height, 
            float x_margin_start, float x_margin_end,
            float y_margin_start, float y_margin_end, 
            size_t elements, GameEngine * engine);

        /**
            Clear the world, DOES NOT destroy the item themselves
            @return true = OK
        */
        int Destroy();

        /**
            Check whether the object is initialised
            @return Is initialised
        */
        bool IsInited();

        /**
            Add a new object T to the sector. The T object should be a subclass of WorldObject
            @param x Position x
            @param y Position y
            @param z Position z
            @param object The opengl model
            @param texture The opengl model texture
            @return A pointer to the object. If nullptr, then allocation failed
        */
        template<typename T> T * NewObj(float x, float y, float z) {
            if (!is_inited_) {
                dt::Console(dt::CRITICAL, "World sector is not initialised");
                return nullptr;
            }

            /* Spawn new object */
            T * the_new_object = new (array_objects_) T();

            /* Insert it into the world */
            size_t index_row = GetColumn(y);
            size_t index_col = GetRow(x);
            world_[index_row][index_col].push_back(the_new_object);
            the_new_object->world_sector_ = this;

            /* Insert it into the physics engine */
            the_new_object->PhysicsObject::Init(x, y, z, physics_engine_);
            the_new_object->SetPosition(x, y, z);
            physics_engine_->Insert(the_new_object);
            
            return the_new_object;
        }

        void UpdateObjectPosition(WorldObject * object, float old_pos_x, float old_pos_y, float new_pos_x, float new_pos_y);

        /**
            Get a window of object in the world. Objects are assigned sequentially to the visible world 
            vector. Does not allocate any new memory
            @param center_x The x coordinate of the window's center
            @param center_y The y coordinate of the window's center
            @param margin The top, down, left, right window margin
            @param[out] visible_world The vector with pointers to the objects window
            @return The number of objects assigned to the visible_world vector
        */
        size_t GetObjectsWindow(float center_x, float center_y, float margin, std::vector<WorldObject *> & visible_world);

    private:
        
        bool is_inited_;
        float x_margin_start_, x_margin_end_, y_margin_start_, y_margin_end_;
        GameEngine * engine_;
        
        /* 
            A struct that resembles the world. Holds pointers to actual objects that are stored 
            sequentially
        */
        physics::PhysicsEngine * physics_engine_;
        std::vector<std::vector<std::deque<WorldObject *> > >world_;
        memory_subsystem::ArrayAllocator * array_objects_;

        /**
        
        */    
        size_t GetRow(float x);
        
        /**
        
        */
        size_t GetColumn(float y);

    };

}

#endif
