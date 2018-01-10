#ifndef __Types_hpp__
#define __Types_hpp__

#include "../HelpFunctions.hpp"

namespace game_engine {
    
    /**
        A point in a 2D pane 
        (x,y) : point coordinates
    */
    typedef struct Point2D {
        float x_;
        float y_;

        Point2D(float x, float y) : x_(x), y_(y) {};
    } Point2D_t;

    /**
        A circle in a 2D pane 
        (x,y) : center coordinates
        r : radius
    */
    typedef struct Circle2D {
        float x_;
        float y_;
        float r_;

        Circle2D(float x, float y, float r) : x_(x), y_(y), r_(r) {};
    } Circle2D_t;

    /**
        A line in a 2D pane 
        [A*x + B*y + C = 0]
    */
    typedef struct Line2D {
        float A_;
        float B_;
        float C_;

        Line2D() {
            A_ = B_ = C_ = 0.0f;
        }
        /**
            Create a line from two points
        */
        Line2D(float a_x, float a_y, float b_x, float b_y) {
            if (Equal(b_x, a_x)) {
                A_ = 1.0f;
                B_ = 0.0f;
                C_ = -a_x;
                return;
            }
            float gradient = (b_y - a_y) / (b_x - a_x);
            A_ = gradient;
            B_ = -1.0f;
            C_ = a_y - gradient*a_x;
        };
        /**
            Create a line from a point and a slope
        */
        Line2D(float a_x, float a_y, float gradient) {
            A_ = gradient;
            B_ = -1.0f;
            C_ = a_y - gradient*a_x;
        }

        /**
        
        */
        float GetGradient() {
            return -A_ / B_;
        }

        /**
        
        */
        float GetYIntercept() {
            return -C_ / A_;
        }

    } Line2D_t;

    /**
        A rectangle in a 2D pane 
        (x,y) : center coordinates 
        x_width: the horizontal-x width
        y_height: the vertical-y height 
    */
    typedef struct Rectangle2D {
        float x_;
        float y_;
        float x_width_;

        float y_height_;
        Rectangle2D(float x, float y, float x_width, float y_height) :
            x_(x), y_(y), x_width_(x_width), y_height_(y_height) {};
    } Rectangle2D_t;

}

#endif