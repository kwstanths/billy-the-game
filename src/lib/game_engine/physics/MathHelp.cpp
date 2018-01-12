#include "MathHelp.hpp"

#include "../HelpFunctions.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;

namespace game_engine {

    float DotProduct(Point2D_t vector_a, Point2D_t vector_b) {
        return vector_a.x_* vector_b.x_ + vector_a.y_ * vector_b.y_;
    }

    bool PointInside(Point2D_t point, Rectangle2D_t rect){
        
        Point2D_t AM(point.x_ - rect.A_.x_, point.y_ - rect.A_.y_);
        Point2D_t AB(rect.B_.x_ - rect.A_.x_, rect.B_.y_ - rect.A_.y_);
        Point2D_t AD(rect.D_.x_ - rect.A_.x_, rect.D_.y_ - rect.A_.y_);

        float dot_product_AM_AB = DotProduct(AM, AB);
        float dot_product_AM_AD = DotProduct(AM, AD);

        if (0 <= dot_product_AM_AB && dot_product_AM_AB < DotProduct(AB, AB) &&
            0 <= dot_product_AM_AD && dot_product_AM_AD < DotProduct(AD, AD)) return true;

        return false;
    }

    bool PointInside(Point2D point, Circle2D_t circle) {

        float distance_to_center = GetDistance(point, circle.c_);        

        if (distance_to_center <= circle.r_) {
            return true;
        }

        return false;
    }

    float GetDistance(Point2D_t point, Line2D_t line) {
        if (Equal(line.A_, 0.0f) && Equal(line.B_, 0.0f)) {
            debug_tools::Console(debug_tools::CRITICAL, "GetDistancePointToLine(): line.A_ and line.B_ are zero");
            return -1.0f;
        }

        return std::abs(line.A_ * point.x_ + line.B_ * point.y_ + line.C_)
            / std::sqrt(line.A_ * line.A_ + line.B_*line.B_);

        return 0.0f;
    }

    float GetDistance(Point2D_t p_a, Point2D_t p_b) {

        float x_diff = p_b.x_ - p_a.x_;
        float y_diff = p_b.y_ - p_a.y_;

        return std::sqrt(x_diff * x_diff + y_diff * y_diff);
    }

    bool IntersectCircleLine(Circle2D_t circle, Line2D_t line) {
        if (Equal(line.A_, 0.0f) && Equal(line.B_, 0.0f)) {
            debug_tools::Console(debug_tools::CRITICAL, "IntersectCircleLine(): line.A_ and line.B_ are zero");
        }

        float distance = GetDistance(circle.c_, line);
        if (distance <= circle.r_) return true;

        return false;
    }

    bool IntersectCircleLineSegment(Circle2D_t circle, Point2D_t point_a, Point2D_t point_b) {

        Line2D_t points_line(point_a.x_, point_a.y_, point_b.x_, point_b.y_);
        /* Check if we intersect at all */
        float distance = GetDistance(circle.c_, points_line);
        if (distance >= circle.r_) return false;
        /* If we do intersect, check the specific line segment */
        float points_line_grad = points_line.GetGradient();

        Line2D_t perpendicular_points_line;
        if (Equal(points_line_grad, 0.0f)) {
            perpendicular_points_line.A_ = 1.0f;
            perpendicular_points_line.B_ = 0.0f;
            perpendicular_points_line.C_ = -circle.c_.x_;
        } else if (isinf(points_line_grad)) {
            perpendicular_points_line.A_ = 0.0f;
            perpendicular_points_line.B_ = 1.0f;
            perpendicular_points_line.C_ = -circle.c_.y_;
        } else {
            float perpendicular_grad = -1.0f / points_line_grad;
            perpendicular_points_line = Line2D_t(circle.c_.x_, circle.c_.y_, perpendicular_grad);
        }

        Point2D_t intersection_point = LinesIntersection(points_line, perpendicular_points_line);

        Point2D_t vector_from_a_to_intersect(intersection_point.x_ - point_a.x_, intersection_point.y_ - point_a.y_);
        Point2D_t vector_from_b_to_intersect(intersection_point.x_ - point_b.x_, intersection_point.y_ - point_b.y_);

        float inner_product = vector_from_a_to_intersect.x_ * vector_from_b_to_intersect.x_ +
            vector_from_a_to_intersect.y_ * vector_from_b_to_intersect.y_;

        if (inner_product <= 0 && distance <= circle.r_) return true;

        return false;
    }

    Point2D_t LinesIntersection(Line2D_t line_a, Line2D_t line_b) {
        float gradient_a = line_a.GetGradient();
        float gradient_b = line_b.GetGradient();
        float yinter_a = line_a.GetYIntercept();
        float yinter_b = line_b.GetYIntercept();

        if (Equal(gradient_a, gradient_b)) {
            debug_tools::Console(debug_tools::CRITICAL, "LinesIntersection(): gradients are almost equal");
        }        
        if (isinf(gradient_a) && isinf(yinter_a)) {
            debug_tools::Console(debug_tools::CRITICAL, "LinesIntersection(): impossible or not initialised line, a");
        }
        if (isinf(gradient_b) && isinf(yinter_b)) {
            debug_tools::Console(debug_tools::CRITICAL, "LinesIntersection(): impossible or not initialised line, b");
        }
        if (isinf(gradient_a) && isinf(yinter_b)) {
            return Point2D_t(-line_a.C_ / line_a.A_, -line_b.C_ / line_b.B_);
        }
        if (isinf(gradient_b) && isinf(yinter_a)) {
            return Point2D_t(-line_b.C_ / line_b.A_, -line_a.C_ / line_a.B_);
        }

        double x = (yinter_b - yinter_a) / (gradient_a - gradient_b);
        double y = gradient_a * x + yinter_a;

        return Point2D_t(x, y);
        
    }

}