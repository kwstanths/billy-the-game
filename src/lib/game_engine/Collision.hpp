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

    /**
        Check for collision between two rectangles
        @param rect_a The first rectangle
        @param rect_b The second rectangle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(Rectangle2D_t rect_a, Rectangle2D_t rect_b);

    /**
        Check for collision between a circle and a rectangle
        @param rect The rectangle
        @param circle The circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(Rectangle2D_t rect, Circle2D_t circle);

    /**
        Check for collision betwwen two circles
        @param circ_a The first circle
        @param circ_b The second circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(Circle2D_t circ_a, Circle2D_t circ_b);

    
    class Collision {
    public:

        virtual bool Check (Collision * other) = 0;

    };

    class CollisionNone : public Collision {
    public:
        bool Check(Collision * other) {
            return false;
        }
    };

    class CollisionBoundingRectangle;
    class CollisionBoundingCircle;

    class CollisionBoundingRectangle : public Collision {
    public:
        CollisionBoundingRectangle(Rectangle2D_t brect): brect_(brect) {};

        Rectangle2D_t brect_;

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);

    };

    class CollisionBoundingCircle : public Collision {
    public:
        CollisionBoundingCircle(Circle2D_t bcircle): bcircle_(bcircle) {};

        Circle2D_t bcircle_;

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);

    };


}


#endif
