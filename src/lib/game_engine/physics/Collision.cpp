#include "Collision.hpp"

#include <cmath>

#include "game_engine/math/Geometry.hpp"

#include "debug_tools/Console.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace physics {

    bool CollisionCheck(math::Rectangle2D rect_a, math::Rectangle2D rect_b) {
        return math::IntersectRect_Rect(rect_a, rect_b);
    }

    bool CollisionCheck(math::Rectangle2D rect, math::Circle2D circle) {
        return math::IntersectRect_Circle(rect, circle);
    }

    bool CollisionCheck(math::Circle2D a, math::Circle2D b) {
        return math::IntersectCircle_Circle(a, b);
    }

    bool CollisionBoundingRectangle::Check(CollisionNone * other) {
        return false;
    }

    bool CollisionBoundingRectangle::Check(CollisionBoundingRectangle * other) {
        return CollisionCheck(brect_, other->brect_);
    }

    bool CollisionBoundingRectangle::Check(CollisionBoundingCircle * other) {
        return CollisionCheck(brect_, other->bcircle_);
    }

    CollisionType CollisionBoundingRectangle::GetType()
    {
        return CollisionType::COLLISION_BOUNDING_RECTANGLE;
    }

    bool CollisionBoundingCircle::Check(CollisionNone * other) {
        return false;
    }

    bool CollisionBoundingCircle::Check(CollisionBoundingRectangle * other) {
        return CollisionCheck(other->brect_, bcircle_);
    }

    bool CollisionBoundingCircle::Check(CollisionBoundingCircle * other) {
        return CollisionCheck(bcircle_, other->bcircle_);
    }

    CollisionType CollisionBoundingCircle::GetType()
    {
        return CollisionType::COLLISION_BOUNDING_CIRCLE;
    }

}
}
