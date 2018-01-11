#ifndef __Collision_hpp__
#define __Collision_hpp__

#include <cstdlib>

#include "physics/Types.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {

    /**
        Types of collision supported
    */
    enum CollisionType {
        COLLISION_NONE,
        COLLISION_BOUNDING_RECTANGLE,
        COLLISION_BOUNDING_CIRCLE,
    };

    /**
        Result of collision detection.
        Each field holds the amount of moving we can do in each direction
    */
    typedef struct {
        float up_ = 0.0f;
        float down_ = 0.0f;
        float left_ = 0.0f;
        float right_ = 0.0f;
    } CollisionResult_t;


    bool CollisionCheck(Rectangle2D_t a, Rectangle2D_t b);

    bool CollisionCheck(Rectangle2D_t a, Circle2D_t b);

    bool CollisionCheck(Circle2D_t a, Circle2D_t b);

    
}


#endif
