#include "Geometry.hpp"

#include "game_engine/math/HelpFunctions.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace math = game_engine::math;

namespace game_engine {
namespace math {

    Real_t DotProduct(Vector2D vector_a, Vector2D vector_b) {
        return vector_a.x()* vector_b.x() + vector_a.y() * vector_b.y();
    }
    
    bool PointInside(Vector2D point, Rectangle2D rect) {

        Vector2D AM({ point.x() - rect.A_.x(), point.y() - rect.A_.y() });
        Vector2D AB({ rect.B_.x() - rect.A_.x(), rect.B_.y() - rect.A_.y() });
        Vector2D AD({ rect.D_.x() - rect.A_.x(), rect.D_.y() - rect.A_.y() });
    
        Real_t dot_product_AM_AB = DotProduct(AM, AB);
        Real_t dot_product_AM_AD = DotProduct(AM, AD);
        Real_t dot_product_AB_AB = DotProduct(AB, AB);
        Real_t dot_product_AD_AD = DotProduct(AD, AD);

        if (0 <= dot_product_AM_AB && 
            dot_product_AM_AB <= dot_product_AB_AB &&
            0 <= dot_product_AM_AD && 
            dot_product_AM_AD <= dot_product_AD_AD) return true;
    
        return false;
    }
    
    bool PointInside(Vector2D point, Circle2D circle) {
    
        Real_t distance_to_center = GetDistance(point, circle.c_);
    
        if (distance_to_center <= circle.r_) {
            return true;
        }
    
        return false;
    }
    
    Real_t GetDistance(Vector2D point, Line2D line) {
        if (math::Equal(line.A_, Real_t(0.0)) && math::Equal(line.B_, Real_t(0.0))) {
            debug_tools::Console(debug_tools::CRITICAL, "GetDistancePointToLine(): line.A_ and line.B_ are zero");
            return -1.0f;
        }
    
        return std::abs(line.A_ * point.x() + line.B_ * point.y() + line.C_)
            / std::sqrt(line.A_ * line.A_ + line.B_*line.B_);
    
        return 0.0f;
    }
    
    Real_t GetDistance(Vector2D p_a, Vector2D p_b) {
    
        Real_t x_diff = p_b.x() - p_a.x();
        Real_t y_diff = p_b.y() - p_a.y();
    
        return std::sqrt(x_diff * x_diff + y_diff * y_diff);
    }
    
    bool IntersectCircle_Line(Circle2D circle, Line2D line) {
        if (math::Equal(line.A_, Real_t(0.0)) && math::Equal(line.B_, Real_t(0.0))) {
            debug_tools::Console(debug_tools::CRITICAL, "IntersectCircle_Line(): line.A_ and line.B_ are zero");
        }
    
        Real_t distance = GetDistance(circle.c_, line);
        if (distance <= circle.r_) return true;
    
        return false;
    }
    
    bool IntersectCircle_LineSegment(Circle2D circle, Vector2D point_a, Vector2D point_b) {
    
        Line2D points_line(point_a.x(), point_a.y(), point_b.x(), point_b.y());
        /* Check if we intersect at all */
        Real_t distance = GetDistance(circle.c_, points_line);
        if (distance >= circle.r_) return false;
        /* If we do intersect, check the specific line segment */
        Real_t points_line_grad = points_line.GetGradient();
    
        Line2D perpendicular_points_line;
        if (math::Equal(points_line_grad, Real_t(0.0))) {
            perpendicular_points_line.A_ = 1.0f;
            perpendicular_points_line.B_ = 0.0f;
            perpendicular_points_line.C_ = -circle.c_.x();
        }
        else if (isinf(points_line_grad)) {
            perpendicular_points_line.A_ = 0.0f;
            perpendicular_points_line.B_ = 1.0f;
            perpendicular_points_line.C_ = -circle.c_.y();
        }
        else {
            Real_t perpendicular_grad = -1.0f / points_line_grad;
            perpendicular_points_line = Line2D(circle.c_.x(), circle.c_.y(), perpendicular_grad);
        }
    
        Vector2D intersection_point = IntersecLine_Line(points_line, perpendicular_points_line);
    
        Vector2D vector_from_a_to_intersect({ intersection_point.x() - point_a.x(), intersection_point.y() - point_a.y() });
        Vector2D vector_from_b_to_intersect({ intersection_point.x() - point_b.x(), intersection_point.y() - point_b.y() });
    
        Real_t inner_product = vector_from_a_to_intersect.x() * vector_from_b_to_intersect.x() +
            vector_from_a_to_intersect.y() * vector_from_b_to_intersect.y();
    
        if (inner_product <= 0 && distance <= circle.r_) return true;
    
        return false;
    }
    
    Vector2D IntersecLine_Line(Line2D line_a, Line2D line_b) {
        Real_t gradient_a = line_a.GetGradient();
        Real_t gradient_b = line_b.GetGradient();
        Real_t yinter_a = line_a.GetYIntercept();
        Real_t yinter_b = line_b.GetYIntercept();
    
        if (math::Equal(gradient_a, gradient_b)) {
            debug_tools::Console(debug_tools::CRITICAL, "IntersecLine_Line(): gradients are almost equal");
        }

        /* Calculate general solution */
        Real_t x_coord, y_coord;
        x_coord = (yinter_b - yinter_a) / (gradient_a - gradient_b);
        y_coord = gradient_a * x_coord + yinter_a;

        /* Special cases */
        if (math::Equal(line_a.B_, Real_t(0.0))) {
            /* Line a is perpendicular to the x-axis */
            x_coord = -line_a.C_ / line_a.A_;
        } else if (math::Equal(line_b.B_, Real_t(0.0))) {
            /* Line b is perpendicular to the x-axis */
            x_coord = -line_b.C_ / line_b.A_;
        }

        if (math::Equal(gradient_a, Real_t(0.0))) {
            /* Line a is perpendicular to the y-axis */
            y_coord = -line_a.C_ / line_a.B_;
        }
        if (math::Equal(gradient_b, Real_t(0.0))) {
            /* line b is perpendicular to the y-axis */
            y_coord = -line_b.C_ / line_b.B_;
        }

        return Vector2D({ x_coord, y_coord });
    }
    
    bool IntersectRect_Rect(Rectangle2D rect_a, Rectangle2D rect_b) {
    
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
    
    bool IntersectRect_Circle(Rectangle2D rect, Circle2D circle) {
    
        if (PointInside(rect.A_, circle)) return true;
        if (PointInside(rect.B_, circle)) return true;
        if (PointInside(rect.C_, circle)) return true;
        if (PointInside(rect.D_, circle)) return true;
        if (IntersectCircle_LineSegment(circle, rect.A_, rect.B_)) return true;
        if (IntersectCircle_LineSegment(circle, rect.B_, rect.C_)) return true;
        if (IntersectCircle_LineSegment(circle, rect.C_, rect.D_)) return true;
        if (IntersectCircle_LineSegment(circle, rect.D_, rect.A_)) return true;
    
        return false;
    }
    
    bool IntersectCircle_Circle(Circle2D circ_a, Circle2D circ_b) {
        Real_t r = circ_a.r_ + circ_b.r_;
        r *= r;
        return r > std::pow(circ_a.c_.x() - circ_b.c_.x(), 2) + std::pow(circ_a.c_.y() - circ_b.c_.y(), 2);
    }

}

}