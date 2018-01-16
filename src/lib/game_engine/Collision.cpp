#include "Collision.hpp"

#include <cmath>

#include "physics/MathHelp.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {

    bool CollisionCheck(Rectangle2D_t rect_a, Rectangle2D_t rect_b) {
        
        if (PointInside(rect_a.A_, rect_b)) return true;
        if (PointInside(rect_a.B_, rect_b)) return true;
        if (PointInside(rect_a.C_, rect_b)) return true;
        if (PointInside(rect_a.D_, rect_b)) return true;
        if (PointInside(rect_b.A_, rect_a)) return true;
        if (PointInside(rect_b.B_, rect_a)) return true;
        if (PointInside(rect_b.C_, rect_a)) return true;
        if (PointInside(rect_b.D_, rect_a)) return true;

        return false;
    }

    bool CollisionCheck(Rectangle2D_t rect, Circle2D_t circle) {
       
        if (PointInside(rect.A_, circle)) return true;
        if (PointInside(rect.B_, circle)) return true;
        if (PointInside(rect.C_, circle)) return true;
        if (PointInside(rect.D_, circle)) return true;
        if (IntersectCircleLineSegment(circle, rect.A_, rect.B_)) return true;
        if (IntersectCircleLineSegment(circle, rect.B_, rect.C_)) return true;
        if (IntersectCircleLineSegment(circle, rect.C_, rect.D_)) return true;
        if (IntersectCircleLineSegment(circle, rect.D_, rect.A_)) return true;

        return false;
    }

    bool CollisionCheck(Circle2D_t a, Circle2D_t b) {
        
        if (GetDistance(a.c_, b.c_) <= a.r_ + b.r_) return true;

        return false;
    }

}
