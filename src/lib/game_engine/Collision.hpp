#ifndef __Collision_hpp__
#define __Collision_hpp__

#include <cstdlib>

#include "physics/Types.hpp"

namespace game_engine {

    /**
        Types of collision supported
    */
    enum CollisionType {
        COLLISION_NONE,
        COLLISION_BOUNDING_RECTANGLE,
        COLLISION_BOUNDING_SPHERE,
    };

    /**
        Collision configuration struct
    */
    typedef struct {
        /* The collision type */
        CollisionType type_;
        /*
            If type_ is NONE, parameter is ignored
            If type_ is BOUNDING_RECTANGLE, parameter is the side length
            If type_ is BOUNDING_SPHERE, parameter is the radius
        */
        float parameter_;
    } CollisionConfig_t;

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

    /**
        Check for collision between two boxes
        @param a The first rectangle
        @param b The second rectangle
        @return true = Boxes collide, false = Boxes do not collide
    */
    bool CollisionCheck2DRectangles(Rectangle2D_t rect_a, Rectangle2D_t rect_b);

    /**
        
    */
    bool CollisionCheck2DCircleRectangle(Circle2D_t circle, Rectangle2D_t rect);

}


#endif
