#ifndef __PhysicsEngine_hpp__
#define __PhysicsEngine_hpp__


#include "game_engine/memory/PoolAllocator.hpp"
#include "game_engine/utility/QuadTree.hpp"
#include "game_engine/utility/CircularBuffer.hpp"
#include "game_engine/math/Types.hpp"

#include "PhysicsObject.hpp"
#include "Collision.hpp"

namespace game_engine {
namespace physics {

    /* The physics engine */
    class PhysicsEngine {
    public:
        /* Does nothing, Call Init() */
        PhysicsEngine();

        /**
            Calls Destroy()
        */
        ~PhysicsEngine();
    
        /**
            Initializes the object
            @param world_size The size of the world
            @param number_of_objects The maximum number of object to hold
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(game_engine::math::Rectangle2D world_size, size_t number_of_objects);

        /**
            Destroys the engine. Deallocates memory used. Not the objects themselves
            @return 0=OK, Never fails, if it does, run for the hills
        */
        int Destroy();
    
        /**
            Get whether the object is initialised
            @return true = initialised, false = not initialised
        */
        bool IsInited();

        /**
        
        */
        void Step();

        /**
            Insert an object to the engine. Will use the object parameters at the moment of calling
            @return 0 = OK, else see ErrorCodes.hpp
        */
        int Insert(PhysicsObject * object);

        /**
            Update the position of an object inside the engine
            @param object The object
            @param new_pos_x The new position horizontal coordinate
            @param new_pos_y The new position vertical coordinate
            @return always 0. If object not found produces a console warning
        */
        int Update(PhysicsObject * object, float new_pos_x, float new_pos_y);

        /**
            Remove an object from the physics engine
            @param object The object to remove
        */
        void Remove(PhysicsObject * object);

        /**
            Finds the neighbour with the highest z inside the search area. Currently pos_x, and pos_y 
            are not used. The idea is to find the one that is also closest to pos_x and pos_y
            @param search_area The aread to search
            @param pos_x The x coordinate of the object that we want to find the closest neighbour
            @param pos_y The y coordinate of the object that we want to find the closest neighbour
            @return A pointer to the closest neighbour, or nullptr if none is found
        */
        size_t GetObjectsArea(math::Rectangle2D search_area, std::vector<PhysicsObject*>& objects);

        /**
            Check for collision inside.
            @param object The object to check
            @param move_offset The amount of moving done
            @param direction The direction of moving
            @return The collision result
        */
        CollisionResult_t CheckCollision(PhysicsObject * object, math::Point2D new_position);
    
    private:

        bool is_inited_;
        
        /* Memory to hold the objects */
        memory_subsystem::PoolAllocator pool_quad_tree_;
        
        /* Data structure to hold the objects */
        utility::QuadTree<PhysicsObject *> world_;
        
        /* Holds the detected collision in each step */
        struct collision_detected_t {
            PhysicsObject * object1_ = nullptr;
            PhysicsObject * object2_ = nullptr;
            collision_detected_t() {};
            collision_detected_t(PhysicsObject * object1, PhysicsObject * object2) : object1_(object1), object2_(object2) {};
        };
        utility::CircularBuffer<collision_detected_t> collisions_;

        /**
            Get the minumum distance between an object and it's surroundings
            @param object The object to check
            @param new_position The new position of the object
            @return A pair of distances to move. First is horisontal, second is vertical
        */
        std::pair<float, float> CollisionGetDistance(PhysicsObject * object, math::Point2D new_position);

        /**
        
        */
        void CallCollisionHandlers();
    };

}

}


#endif