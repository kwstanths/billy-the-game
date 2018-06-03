#ifndef __Geometry_hpp__
#define __Geometry_hpp__

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "Types.hpp"

namespace game_engine {

namespace physics {

    /**
        Find the inner product between two vectors defined by points
        @param vector_b First vector
        @param vector_a Second vector
        @return The inner product
    */
    float DotProduct(Point2D vector_a, Point2D vector_b);
    
    /**
        Check whether a point is inside a rectangle in a 2D pane
        @param point The point
        @param rect The rectangle
        @return true = inside, false = not inside
    */
    bool PointInside(Point2D point, Rectangle2D rect);
    
    /**
        Check whether a point is inside a circle in a 2D pane
        @param point The point
        @param circle The circle
        @return true = inside, false = not inside
    */
    bool PointInside(Point2D point, Circle2D circle);
    
    /**
        Get the distance between a point and line in a 2D pane
        @param point The point
        @param line The line
        @return The distance
    */
    float GetDistance(Point2D point, Line2D line);
    
    /**
        Get the distance between two points
        @param p_a The first point
        @param p_b The second point
        @return The distance
    */
    float GetDistance(Point2D p_a, Point2D p_b);
    
    /**
        Check whether a line intersects with a circle, the line is an infinite line
        @param circle The circle
        @param line The line
        @return true = intersects, false = Does not intersect
    */
    bool IntersectCircle_Line(Circle2D circle, Line2D line);
    
    /**
        Check whether a line segment intersects with a circle, the line is contained
        between two points
        @param circle The circle
        @param point_a The line segment first point
        @param point_b The line segment second point
        @return true = intersects, false = Does not intersect
    */
    bool IntersectCircle_LineSegment(Circle2D circle, Point2D point_a, Point2D point_b);
    
    /**
        Get the point of intersection between two lines
        @param line_a The first line
        @param line_b The second line
        @return The intersection point, infs will be returned in parallel lines
    */
    Point2D IntersecLine_Line(Line2D line_a, Line2D line_b);
    
    /**
        Check for intersection between two rectangles
        @param rect The first rectangle
        @param circle The second rectnalge
        @return true = Collide, false = do not collide
    */
    bool IntersectRect_Rect(Rectangle2D rect_a, Rectangle2D rect_b);
    
    /**
        Check for intersection between a rectangle and a circle
        @param circ_a The rectangle
        @param circ_b The circle
        @return true = Collide, false = do not collide
    */
    bool IntersectRect_Circle(Rectangle2D rect, Circle2D circle);
    
    /**
        Check for intersection between two circles
        @param circ_a The first circle
        @param circ_b The second circle
        @return true = Collide, false = do not collide
    */
    bool IntersectCircle_Circle(Circle2D circ_a, Circle2D circ_b);

}

}

#endif