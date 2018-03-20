#ifndef __Types_hpp__
#define __Types_hpp__

#include "HelpFunctions.hpp"

namespace game_engine {
    
    /* Shows a direction in degrees: 0 = top, 90 = left, 180 = bottom, 270 = right */
    typedef float Direction;

    /**
        A point in a 2D pane 
        (x,y) : point coordinates
    */
    struct Point2D_t {
        float x_;
        float y_;

        Point2D_t() {
            x_ = y_ = 0.0f;
        }
        Point2D_t(float x, float y) : x_(x), y_(y) {};

        bool operator==(Point2D_t a) const {
            if (Equal(a.x_, x_) && Equal(a.y_, y_)) return true;
            else return false;
        }

    };

    /**
        A circle in a 2D pane 
        c : center point
        r : radius
    */
    struct Circle2D_t {
        Point2D_t c_;
        float r_;


        Circle2D_t() {};
        Circle2D_t(float x, float y, float r) {
            c_.x_ = x;
            c_.y_ = y;
            r_ = r;
        };
    };

    /**
        A line in a 2D pane 
        [A*x + B*y + C = 0]
    */
    struct Line2D_t {
        float A_;
        float B_;
        float C_;

        Line2D_t() {
            A_ = B_ = C_ = 0.0f;
        }
        /**
            Create a line from two points
        */
        Line2D_t(float a_x, float a_y, float b_x, float b_y) {
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
        Line2D_t(float a_x, float a_y, float gradient) {
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

    };

    /**
        A rectangle in a 2D pane defined by four points
        D   C
        A   B
    */
    struct Rectangle2D_t {
        Point2D_t A_;
        Point2D_t B_;
        Point2D_t C_;
        Point2D_t D_;

        Rectangle2D_t() {};

        Rectangle2D_t(Point2D_t A, Point2D_t B, Point2D_t C, Point2D_t D): A_(A), B_(B), C_(C), D_(D) {};
        
        Rectangle2D_t(float center_x, float center_y, float x_width, float y_height) {
            float xmar = x_width / 2.0f;
            float ymar = y_height / 2.0f;

            A_ = Point2D_t(center_x - xmar, center_y - ymar);
            B_ = Point2D_t(center_x + xmar, center_y - ymar);
            C_ = Point2D_t(center_x + xmar, center_y + ymar);
            D_ = Point2D_t(center_x - xmar, center_y + ymar);
        }

    };

}

#endif