#ifndef __MathHelp_hpp__
#define __MathHelp_hpp__

#include "Types.hpp"

namespace game_engine {

    /**
        Check whether a point is inside a rectangle in a 2D pane
        @param point The point
        @param rect The rectangle
        @return true = inside, false = not inside
    */
    bool PointInsideRectangle(Point2D_t point, Rectangle2D_t rect);

    /**
        Get the distance between a point and line in a 2D pane
        @param point The point
        @param line The line
        @return The distance
    */
    float GetDistancePointToLine(Point2D_t point, Line2D_t line);

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