#include "Collision.hpp"

#include <cmath>

#include "physics/MathHelp.hpp"

namespace game_engine {

    bool CollisionCheck2DRectangles(Rectangle2D_t a, Rectangle2D_t b) {
        if ((std::abs(a.x_ - b.x_) * 2.0 < (a.x_width_ + b.x_width_)) && (std::abs(a.y_ - b.y_) * 2.0 < (a.y_height_ + b.y_height_))) {
            return true;
        }
        
        return false;
    }

    bool CollisionCheck2DCircleRectangle(Circle2D_t a, Rectangle2D_t b) {
        float xmar = b.x_width_ / 2.0f;
        float ymar = b.y_height_ / 2.0f;

        Point2D_t point_A(b.x_ - xmar, b.y_ - ymar);
        Point2D_t point_B(b.x_ + xmar, b.y_ - ymar);
        Point2D_t point_C(b.x_ + xmar, b.y_ + ymar);
        Point2D_t point_D(b.x_ - xmar, b.y_ + ymar);

        return PointInsideRectangle(Point2D_t(a.x_, a.y_), b) ||
            IntersectCircleLineSegment(a, point_A, point_B) ||
            IntersectCircleLineSegment(a, point_B, point_C) ||
            IntersectCircleLineSegment(a, point_C, point_D) ||
            IntersectCircleLineSegment(a, point_D, point_A);

    }

}
