#ifndef __Types_hpp__
#define __Types_hpp__

#include "../HelpFunctions.hpp"

namespace game_engine {
    
    /* Shows a direction in degrees: 0 = top, 90 = left, 180 = bottom, 270 = right */
    typedef float Direction;

    /**
        A point in a 2D pane 
        (x,y) : point coordinates
    */
    typedef struct Point2D {
        float x_;
        float y_;

        Point2D() {
            x_ = y_ = 0.0f;
        }
        Point2D(float x, float y) : x_(x), y_(y) {};
    } Point2D_t;

    /**
        A circle in a 2D pane 
        c : center point
        r : radius
    */
    typedef struct Circle2D {
        Point2D_t c_;
        float r_;


        Circle2D() {};
        Circle2D(float x, float y, float r) {
            c_.x_ = x;
            c_.y_ = y;
            r_ = r;
        };
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
            Get the line gradient
        */
        float GetGradient() {
            return -A_ / B_;
        }

        /**
            Get the line y-intercept
        */
        float GetYIntercept() {
            return -C_ / A_;
        }

    } Line2D_t;

    /**
        A rectangle in a 2D pane defined by four points
        D   C
        A   B
    */
    typedef struct Rectangle2D {
        Point2D_t A_;
        Point2D_t B_;
        Point2D_t C_;
        Point2D_t D_;

        Rectangle2D() {};

        Rectangle2D(Point2D_t A, Point2D_t B, Point2D_t C, Point2D_t D): A_(A), B_(B), C_(C), D_(D) {};
        
        Rectangle2D(float center_x, float center_y, float x_width, float y_height) {
            float xmar = x_width / 2.0f;
            float ymar = y_height / 2.0f;

            A_ = Point2D_t(center_x - xmar, center_y - ymar);
            B_ = Point2D_t(center_x + xmar, center_y - ymar);
            C_ = Point2D_t(center_x + xmar, center_y + ymar);
            D_ = Point2D_t(center_x - xmar, center_y + ymar);
        }

    } Rectangle2D_t;

}

#endif