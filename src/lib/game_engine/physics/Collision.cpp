#include "Collision.hpp"

#include <cmath>

#include "game_engine/math/Geometry.hpp"
#include "debug_tools/Assert.hpp"

#include "debug_tools/Console.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace physics {

    bool CollisionCheck(game_engine::math::AABox<2> box_a, game_engine::math::AABox<2> box_b) {
        return box_a.Overlaps(box_b);
    }

    bool CollisionCheck(game_engine::math::AABox<2> box, game_engine::math::Circle2D circle) {
        _assert(0);
        return false;
    }

    bool CollisionCheck(math::Circle2D a, math::Circle2D b) {
        return math::IntersectCircle_Circle(a, b);
    }

    bool CollisionBoundingBox::Check(CollisionNone * other) {
        return false;
    }

    bool CollisionBoundingBox::Check(CollisionBoundingBox * other) {
        return CollisionCheck(bbox_, other->bbox_);
    }

    bool CollisionBoundingBox::Check(CollisionBoundingCircle * other) {
        return CollisionCheck(bbox_, other->bcircle_);
    }

    CollisionType CollisionBoundingBox::GetType()
    {
        return CollisionType::COLLISION_BOUNDING_BOX;
    }

    bool CollisionBoundingCircle::Check(CollisionNone * other) {
        return false;
    }

    bool CollisionBoundingCircle::Check(CollisionBoundingBox * other) {
        return CollisionCheck(other->bbox_, bcircle_);
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
