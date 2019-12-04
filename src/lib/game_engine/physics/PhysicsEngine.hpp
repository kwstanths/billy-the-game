#ifndef __PhysicsEngine_hpp__
#define __PhysicsEngine_hpp__

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
        int Init(game_engine::math::AABox<2> world_size, size_t number_of_objects);

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
        int Insert(PhysicsObject * object, math::Vector2D& position);

        /**
            Update the position of an object inside the engine
            @param object The object
            @param new_pos_x The new position horizontal coordinate
            @param new_pos_y The new position vertical coordinate
            @return always 0. If object not found produces a console warning
        */
        int Update(PhysicsObject * object, math::Vector2D& new_position);

        /**
            Remove an object from the physics engine
            @param object The object to remove
        */
        void Remove(PhysicsObject * object);

        void GetObjectsArea(math::AABox<2> search_area, std::vector<PhysicsObject*>& objects);

        /**
            Check for collision inside.
            @param object The object to check
            @param move_offset The amount of moving done
            @param direction The direction of moving
            @return The collision result
        */
        math::Vector2D CheckCollision(PhysicsObject * object, math::Vector2D new_position);
    
    private:

        bool is_inited_;
        
        /* Data structure to hold the objects */
        utility::QuadTree<PhysicsObject *> * world_;

    };

}

}


#endif