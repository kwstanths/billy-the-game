#include "Collision.hpp"

#include <cmath>

#include "physics/Geometry.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {

    bool CollisionCheck(Rectangle2D_t rect_a, Rectangle2D_t rect_b) {
        return IntersectRect_Rect(rect_a, rect_b);
    }

    bool CollisionCheck(Rectangle2D_t rect, Circle2D_t circle) {
        return IntersectRect_Circle(rect, circle);
    }

    bool CollisionCheck(Circle2D_t a, Circle2D_t b) {
        
        if (GetDistance(a.c_, b.c_) <= a.r_ + b.r_) return true;

        return false;
    }

    bool CollisionBoundingRectangle::Check(CollisionNone * other){
        return false;
    }

    bool CollisionBoundingRectangle::Check(CollisionBoundingRectangle * other){
        return CollisionCheck(brect_, other->brect_);
    }
    
    bool CollisionBoundingRectangle::Check(CollisionBoundingCircle * other){
        return CollisionCheck(brect_, other->bcircle_);
    }

    bool CollisionBoundingCircle::Check(CollisionNone * other){
        return false;
    }
    
    bool CollisionBoundingCircle::Check(CollisionBoundingRectangle * other){
        return CollisionCheck(other->brect_, bcircle_);
    }
    
    bool CollisionBoundingCircle::Check(CollisionBoundingCircle * other){
        return CollisionCheck(bcircle_, other->bcircle_);
    }


}
