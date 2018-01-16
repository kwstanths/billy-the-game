#ifndef __MathHelp_hpp__
#define __MathHelp_hpp__

#include "Types.hpp"

namespace game_engine {

    /**
        Find the inner product between two vectors defined by points
        @param vector_b First vector
        @param vector_a Second vector
        @return The inner product
    */
    float DotProduct(Point2D_t vector_a, Point2D_t vector_b);

    /**
        Check whether a point is inside a rectangle in a 2D pane
        @param point The point
        @param rect The rectangle
        @return true = inside, false = not inside
    */
    bool PointInside(Point2D_t point, Rectangle2D_t rect);

    /**
        Check whether a point is inside a circle in a 2D pane
        @param point The point
        @param circle The circle
        @return true = inside, false = not inside
    */
    bool PointInside(Point2D point, Circle2D_t circle);

    /**
        Get the distance between a point and line in a 2D pane
        @param point The point
        @param line The line
        @return The distance
    */
    float GetDistance(Point2D_t point, Line2D_t line);

    /**
        Get the distance between two points
        @param p_a The first point
        @param p_b The second point
        @return The distance
    */
    float GetDistance(Point2D_t p_a, Point2D_t p_b);

    /**
        Check whether a line intersects with a circle, the line is an infinite line
        @param circle The circle
        @param line The line
        @return true = intersects, false = Does not intersect
    */
    bool IntersectCircleLine(Circle2D_t circle, Line2D_t line);

    /**
        Check whether a line segment intersects with a circle, the line is contained
        between two points
        @param circle The circle
        @param point_a The line segment first point
        @param point_b The line segment second point
        @return true = intersects, false = Does not intersect
    */
    bool IntersectCircleLineSegment(Circle2D_t circle, Point2D_t point_a, Point2D_t point_b);

    /**
        Get the point of intersection between two lines
        @param line_a The first line
        @param line_b The second line
        @return The intersection point, infs will be returned in parallel lines
    */
    Point2D_t LinesIntersection(Line2D_t line_a, Line2D_t line_b);

}

#endif