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
    bool CollisionCheck(Rectangle2D rect_a, Rectangle2D rect_b);

    /**
        Check for collision between a circle and a rectangle
        @param rect The rectangle
        @param circle The circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(Rectangle2D rect, Circle2D circle);

    /**
        Check for collision betwwen two circles
        @param circ_a The first circle
        @param circ_b The second circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(Circle2D circ_a, Circle2D circ_b);


    class CollisionNone;
    class CollisionBoundingRectangle;
    class CollisionBoundingCircle;
    
    class Collision {
    public:
        virtual bool Check(Collision * other) = 0;
        virtual bool Check(CollisionNone * other) = 0;
        virtual bool Check(CollisionBoundingCircle * other) = 0;
        virtual bool Check(CollisionBoundingRectangle * other) = 0;

        virtual CollisionType GetType() = 0;

        virtual void Translate(float x, float y) = 0;
        virtual void Rotate(float th) = 0;
    };

    class CollisionNone : public Collision {
    public:
        bool Check(Collision * other) {
            return other->Check(this);
        }
        bool Check(CollisionNone * other) {
            return false;
        }
        bool Check(CollisionBoundingCircle * other) {
            return false;
        }
        bool Check(CollisionBoundingRectangle * other) {
            return false;
        }
        CollisionType GetType() {
            return CollisionType::COLLISION_NONE;
        }
        void Translate(float x, float y) {
            return;
        }
        void Rotate(float th) {
            return;
        }
    };

    class CollisionBoundingRectangle : public Collision {
    public:
        Rectangle2D brect_;

        CollisionBoundingRectangle(Rectangle2D brect): brect_(brect) {};

        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);
        
        CollisionType GetType();
        
        void Translate(float x, float y) {
            brect_.Translate(x, y);
        }

        void Rotate(float th) {
            brect_.Rotate(th);
        }
    };

    class CollisionBoundingCircle : public Collision {
    public:
        CollisionBoundingCircle(Circle2D bcircle): bcircle_(bcircle) {};

        Circle2D bcircle_;
        
        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);
        
        CollisionType GetType();

        void Translate(float x, float y) {
            bcircle_.Translate(x, y);
        }

        void Rotate(float th) {
            bcircle_.Rotate(th);
        }
    };


}


#endif
