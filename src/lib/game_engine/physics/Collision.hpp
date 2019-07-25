#ifndef __Collision_hpp__
#define __Collision_hpp__

#include <cstdlib>
#include <exception>

#include "game_engine/math/Types.hpp"
#include "game_engine/math/Point.hpp"
#include "game_engine/math/AABox.hpp"
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
        COLLISION_BOUNDING_BOX,
        COLLISION_BOUNDING_CIRCLE,
    };

    /**
        Check for collision between two rectangles
        @param rect_a The first rectangle
        @param rect_b The second rectangle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::AABox<2> box_a, game_engine::math::AABox<2> box_b);

    /**
        Check for collision between a circle and a rectangle
        @param rect The rectangle
        @param circle The circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::AABox<2> box, game_engine::math::Circle2D circle);

    /**
        Check for collision betwwen two circles
        @param circ_a The first circle
        @param circ_b The second circle
        @return true = Collide, false = do not collide
    */
    bool CollisionCheck(game_engine::math::Circle2D circ_a, game_engine::math::Circle2D circ_b);


    class CollisionNone;
    class CollisionBoundingBox;
    class CollisionBoundingCircle;

    class Collision {
    public:
        virtual bool Check(Collision * other) = 0;
        virtual bool Check(CollisionNone * other) = 0;
        virtual bool Check(CollisionBoundingCircle * other) = 0;
        virtual bool Check(CollisionBoundingBox * other) = 0;

        virtual void Translate(game_engine::Real_t x, game_engine::Real_t y) = 0;
        virtual CollisionType GetType() = 0;
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
        bool Check(CollisionBoundingBox * other) {
            return false;
        }
        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            return;
        }
        CollisionType GetType() {
            return CollisionType::COLLISION_NONE;
        }
    };

    class CollisionBoundingBox : public Collision {
        friend CollisionBoundingCircle;
    public:
        CollisionBoundingBox(game_engine::math::AABox<2> bbox) : bbox_(bbox) {};

        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingBox * other);

        bool Check(CollisionBoundingCircle * other);
        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            bbox_.Translate(math::Point2D({ x, y }));
        }
        CollisionType GetType();

    private:
        game_engine::math::AABox<2> bbox_;
    };

    class CollisionBoundingCircle : public Collision {
        friend CollisionBoundingBox;
    public:
        CollisionBoundingCircle(game_engine::math::Circle2D bcircle) : bcircle_(bcircle) {};

        bool Check(Collision * other) {
            return other->Check(this);
        }

        bool Check(CollisionNone * other);

        bool Check(CollisionBoundingBox * other);

        bool Check(CollisionBoundingCircle * other);
        
        void Translate(game_engine::Real_t x, game_engine::Real_t y) {
            bcircle_.Translate(x, y);
        }
        
        CollisionType GetType();

    private:
        game_engine::math::Circle2D bcircle_;
    };

}
}


#endif
