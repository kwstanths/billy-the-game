#include "MathHelp.hpp"

#include "../HelpFunctions.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;

namespace game_engine {

    bool PointInsideRectangle(Point2D_t point, Rectangle2D_t rect){
        
        //if (2.0*rect.x_ - rect.x_width_ <= 2.0*point.x_ &&  2.0*point.x_ <= 2.0*rect.x_ + rect.x_width_ &&
        //    2.0*rect.y_ - rect.y_height_ <= 2.0*point.y_ && 2.0*point.y_ <= 2.0*rect.y_ + rect.y_height_)
        //{
        //    return true;
        //}

        return false;
    }

    float GetDistancePointToLine(Point2D_t point, Line2D_t line) {
        if (Equal(line.A_, 0.0f) && Equal(line.B_, 0.0f)) {
            debug_tools::Console(debug_tools::CRITICAL, "GetDistancePointToLine(): line.A_ and line.B_ are zero");
            return -1.0f;
        }

        return std::abs(line.A_ * point.x_ + line.B_ * point.y_ + line.C_)
            / std::sqrt(line.A_ * line.A_ + line.B_*line.B_);

        return 0.0f;
    }

    bool IntersectCircleLine(Circle2D_t circle, Line2D_t line) {
        if (Equal(line.A_, 0.0f) && Equal(line.B_, 0.0f)) {
            debug_tools::Console(debug_tools::CRITICAL, "IntersectCircleLine(): line.A_ and line.B_ are zero");
        }

        float distance = GetDistancePointToLine(Point2D_t(circle.x_, circle.y_), line);
        if (distance <= circle.r_) return true;

        return false;
    }

    bool IntersectCircleLineSegment(Circle2D_t circle, Point2D_t point_a, Point2D_t point_b) {

        Line2D_t points_line(point_a.x_, point_a.y_, point_b.x_, point_b.y_);
        /* Check if we intersect at all */
        if (!IntersectCircleLine(circle, points_line)) return false;
        /* If we do intersect, check the specific line segment */
        float points_line_grad = points_line.GetGradient();

        Line2D_t perpendicular_points_line;
        if (Equal(points_line_grad, 0.0f)) {
            perpendicular_points_line.A_ = 1.0f;
            perpendicular_points_line.B_ = 0.0f;
            perpendicular_points_line.C_ = -circle.x_;
        } else if (isinf(points_line_grad)) {
            perpendicular_points_line.A_ = 0.0f;
            perpendicular_points_line.B_ = 1.0f;
            perpendicular_points_line.C_ = -circle.y_;
        } else {
            float perpendicular_grad = -1.0f / points_line_grad;
            perpendicular_points_line = Line2D_t(circle.x_, circle.y_, perpendicular_grad);
        }

        Point2D_t intersection_point = LinesIntersection(points_line, perpendicular_points_line);

        Point2D_t vector_from_a_to_intersect(intersection_point.x_ - point_a.x_, intersection_point.y_ - point_a.y_);
        Point2D_t vector_from_b_to_intersect(intersection_point.x_ - point_b.x_, intersection_point.y_ - point_b.y_);

        float inner_product = vector_from_a_to_intersect.x_ * vector_from_b_to_intersect.x_ +
            vector_from_a_to_intersect.y_ * vector_from_b_to_intersect.y_;

        dt::ConsoleInfo("point a x", point_a.x_,
            "point a y", point_a.y_,
            "point b x", point_b.x_,
            "point b y", point_b.y_,
            "inter x", intersection_point.x_,
            "inter y", intersection_point.y_,
            "product", inner_product);

        if (inner_product <= 0) return true;

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