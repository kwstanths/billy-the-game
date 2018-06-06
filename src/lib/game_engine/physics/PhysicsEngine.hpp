#ifndef __PhysicsEngine_hpp__
#define __PhysicsEngine_hpp__

#include "PhysicsObject.hpp"

#include "game_engine/memory/PoolAllocator.hpp"

#include "game_engine/utility/QuadTree.hpp"

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
        int Init(Rectangle2D world_size, size_t number_of_objects);

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
            Check for collision inside.
            @param object The object to check
            @param move_offset The amount of moving done
            @param direction The direction of moving
            @return The collision result
        */
        CollisionResult_t CheckCollision(PhysicsObject * object, float move_offset, Direction direction);
    
    private:
        bool is_inited_;
        /* Memory to hold the objects */
        memory_subsystem::PoolAllocator pool_quad_tree_;
        /* Holds the objects */
        utility::QuadTree<PhysicsObject> world_;

        /**
            Get the minumum distance between an object and it's surroundings
            @param object The object to check
            @param new_position The new position of the object
            @return A pair of distances to move. First is horisontal, second is vertical
        */
        std::pair<float, float> CollisionGetDistance(PhysicsObject * object, Point2D new_position);
    };

}

}


#endif