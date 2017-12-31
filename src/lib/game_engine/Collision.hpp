#ifndef __Collision_hpp__
#define __Collision_hpp__

#include <cstdlib>

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
        size_t parameter_;
    } CollisionConfig_t;

    /**
        Result of collision detection
    */
    typedef struct {
        bool move_up = false;
        bool move_down = false;
        bool move_left = false;
        bool move_right = false;
    } CollisionResult_t;

    /**
        Check for collision between two boxes
        @param a_x The first box center x coordinate
        @param a_y The first box center y coordinate
        @param a_x_width The first box x-axis width
        @param a_y_height The first box y-axis height
        @param b_x The second box center x coordinate
        @param b_y The second box center y coordinate
        @param b_x_width The second box x-axis width
        @param b_y_height The second box y-axis height
        @return true = Boxes collide, false = Boxes do not collide
    */
    bool CollisionCheck2DRectangles(float a_x, float a_y, float a_x_width, float a_y_height,
        float b_x, float b_y, float b_x_width, float b_y_height);

}


#endif
