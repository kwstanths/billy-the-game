#ifndef __MathHelp_hpp__
#define __MathHelp_hpp__

#include "Types.hpp"

namespace game_engine {

    /**
        Find the innter product between two vectors defined by two points
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
        
    */
    bool IntersectCircleLineSegment(Circle2D_t circle, Point2D_t point_a, Point2D_t point_b);

    /**
        
    */
    Point2D_t LinesIntersection(Line2D_t line_a, Line2D_t line_b);

}

#endif