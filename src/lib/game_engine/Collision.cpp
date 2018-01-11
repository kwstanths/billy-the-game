#include "Collision.hpp"

#include <cmath>

#include "physics/MathHelp.hpp"

namespace game_engine {

    /*bool CollisionCheck(Rectangle2D_t a, Rectangle2D_t b) {
        if ((std::abs(a.x_ - b.x_) * 2.0 < (a.x_width_ + b.x_width_)) && (std::abs(a.y_ - b.y_) * 2.0 < (a.y_height_ + b.y_height_))) {
            return true;
        }
        
        return false;
    }

    bool CollisionCheck(Circle2D_t a, Rectangle2D_t b) {
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

    }*/

    bool CollisionCheck(Rectangle2D_t rect_a, Rectangle2D_t rect_b) {
        
        bool a_is_left_of_b = rect_a.B_.x_ <= rect_b.A_.x_;
        bool a_is_right_of_b = rect_a.A_.x_ >= rect_b.B_.x_;
        bool a_is_bellow_of_b = rect_a.D_.y_ <= rect_b.A_.y_;
        bool a_is_top_of_b = rect_a.A_.y_ >= rect_b.D_.y_;

        bool collision = a_is_left_of_b || a_is_right_of_b || a_is_bellow_of_b || a_is_top_of_b;

        return !collision;
    }

    bool CollisionCheck(Rectangle2D_t a, Circle2D_t b) {
        return false;
    }

    bool CollisionCheck(Circle2D_t a, Circle2D_t b) {
        return false;
    }

}
