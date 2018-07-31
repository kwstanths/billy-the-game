#ifndef __Collision_hpp__
#define __Collision_hpp__

#include <cstdlib>
#include <exception>

#include "game_engine/math/Types.hpp"
#include "game_engine/memory/ArrayAllocator.hpp"
#include "game_engine/memory/PoolAllocator.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {
namespace physics {

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
        game_engine::Real_t horizontal_ = 0.0f;
        game_engine::Real_t vertical_ = 0.0f;
    } CollisionResult_t;

    /**
        Check for collision between two rectangles
        @param rect_a The first rectangle
        @param rect_b The second rectangle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::Rectangle2D rect_a, game_engine::math::Rectangle2D rect_b);

    /**
        Check for collision between a circle and a rectangle
        @param rect The rectangle
        @param circle The circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::Rectangle2D rect, game_engine::math::Circle2D circle);

    /**
        Check for collision betwwen two circles
        @param circ_a The first circle
        @param circ_b The second circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::Circle2D circ_a, game_engine::math::Circle2D circ_b);


    class CollisionNone;
    class CollisionBoundingRectangle;
    class CollisionBoundingCircle;

    class Collision {
    public:
        virtual bool Check(Collision * other) = 0;
        virtual bool Check(CollisionNone * other) = 0;
        virtual bool Check(CollisionBoundingCircle * other) = 0;
        virtual bool Check(CollisionBoundingRectangle * other) = 0;

        virtual math::Shape2D * GetShape() = 0;
        virtual CollisionType GetType() = 0;

        virtual void Translate(game_engine::Real_t x, game_engine::Real_t y) = 0;
        virtual void Rotate(game_engine::Real_t th) = 0;
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
        math::Shape2D * GetShape() { 
            return nullptr; 
        }
        CollisionType GetType() {
            return CollisionType::COLLISION_NONE;
        }
        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            return;
        }
        void Rotate(game_engine::Real_t th) {
            return;
        }
    };

    class CollisionBoundingRectangle : public Collision {
        friend CollisionBoundingCircle;
    public:
        CollisionBoundingRectangle(game_engine::math::Rectangle2D brect) : brect_(brect) {};

        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);

        math::Shape2D * GetShape() {
            return &brect_;
        }

        CollisionType GetType();

        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            brect_.Translate(x, y);
        }

        void Rotate(game_engine::Real_t th) {
            brect_.Rotate(th);
        }
    private:
        game_engine::math::Rectangle2D brect_;
    };

    class CollisionBoundingCircle : public Collision {
        friend CollisionBoundingRectangle;
    public:
        CollisionBoundingCircle(game_engine::math::Circle2D bcircle) : bcircle_(bcircle) {};

        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingRectangle * other);

        bool Check(CollisionBoundingCircle * other);

        math::Shape2D * GetShape() {
            return &bcircle_;
        }

        CollisionType GetType();

        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            bcircle_.Translate(x, y);
        }

        void Rotate(game_engine::Real_t th) {
            bcircle_.Rotate(th);
        }
    private:
        game_engine::math::Circle2D bcircle_;
    };

}
}


#endif
