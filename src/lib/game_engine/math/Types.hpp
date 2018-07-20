#ifndef __Types_hpp__
#define __Types_hpp__

#include <iostream>

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
   

namespace game_engine {

    typedef float Real_t;
    /* Shows a direction in degrees: 0 = top, 90 = left, 180 = bottom, 270 = right */
    typedef float Direction_t;

namespace math {

    class Shape2D {
        virtual void Translate(Real_t x, Real_t y) = 0;
        virtual void Rotate(Real_t th) = 0;
    };


    /**
        A point in a 2D pane
        (x,y) : point coordinates
    */
    class Point2D {
    public:
        Real_t x_;
        Real_t y_;

        Point2D() {
            x_ = y_ = 0.0f;
        }
        Point2D(Real_t x, Real_t y) : x_(x), y_(y) {};

        bool operator==(Point2D a) const;

        /*
            Translate the point
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(Real_t x, Real_t y) {
            x_ += x;
            y_ += y;
        }

        /**
            Rotate the point clockwise around another point
            @param th The rotation angle in radians
            @param axis The point to rotate around
        */
        void Rotate(Real_t th, Point2D axis) {
            Real_t c = cos(th);
            Real_t s = sin(th);

            Real_t x = x_ - axis.x_;
            Real_t y = y_ - axis.y_;

            x_ = x * c + y * s;
            y_ = -x * s + y * c;

            x_ += axis.x_;
            y_ += axis.y_;
        }

        friend std::ostream& operator<<(std::ostream& os, const Point2D& p);

    };

    /**
        A circle in a 2D pane
        c : center point
        r : radius
    */
    class Circle2D : public Shape2D {
    public:
        Point2D c_;
        Real_t r_;


        Circle2D() {};
        Circle2D(Real_t x, Real_t y, Real_t r) {
            c_.x_ = x;
            c_.y_ = y;
            r_ = r;
        };

        /**
            Translate the circle
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(Real_t x, Real_t y) {
            c_.Translate(x, y);
        }

        /**
            Scale the circle
            @param s The scaling amount
        */
        void Scale(Real_t s) {
            r_ = r_ * s;
        }

        /**
            Rotate the circle clockwise around its center
            @param th The rotation angle in radians
        */
        void Rotate(Real_t th) {
            /* Do nothing */
        }
    };

    /**
        A rectangle in a 2D pane defined by four points
        D   C
        A   B
    */
    class Rectangle2D : public Shape2D {
    public:
        Point2D A_;
        Point2D B_;
        Point2D C_;
        Point2D D_;

        Rectangle2D() {};

        Rectangle2D(Point2D A, Point2D B, Point2D C, Point2D D) : A_(A), B_(B), C_(C), D_(D) {};

        Rectangle2D(Real_t center_x, Real_t center_y, Real_t x_width, Real_t y_height) {
            Real_t xmar = x_width / 2.0f;
            Real_t ymar = y_height / 2.0f;

            A_ = Point2D(center_x - xmar, center_y - ymar);
            B_ = Point2D(center_x + xmar, center_y - ymar);
            C_ = Point2D(center_x + xmar, center_y + ymar);
            D_ = Point2D(center_x - xmar, center_y + ymar);
        }

        /*
            Translate the rectangle
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(Real_t x, Real_t y) {
            A_.Translate(x, y);
            B_.Translate(x, y);
            C_.Translate(x, y);
            D_.Translate(x, y);
        }

        /**
            Scale the rectangle
            @param s The scaling amount
        */
        void Scale(Real_t s) {
            dt::Console(dt::CRITICAL, "Rectangle2D::Scale(): This function is not implemented");
        }

        /**
            Rotate the rectangle clockwise around its center
            @param th The rotation angle in radians
        */
        void Rotate(Real_t th) {
            Point2D center((B_.x_ + D_.x_) / 2.0f, (B_.y_ + D_.y_) / 2.0f);

            A_.Rotate(th, center);
            B_.Rotate(th, center);
            C_.Rotate(th, center);
            D_.Rotate(th, center);
        }

    };

    /**
        A line in a 2D pane
        [A*x + B*y + C = 0]
    */
    class Line2D {
    public:
        Real_t A_;
        Real_t B_;
        Real_t C_;

        Line2D() {
            A_ = B_ = C_ = 0.0f;
        }
        /**
            Create a line from two points
        */
        Line2D(Real_t a_x, Real_t a_y, Real_t b_x, Real_t b_y);
        /**
            Create a line from a point and a slope
        */
        Line2D(Real_t a_x, Real_t a_y, Real_t gradient) {
            A_ = gradient;
            B_ = -1.0f;
            C_ = a_y - gradient * a_x;
        }

        /**
            Get the line gradient
        */
        Real_t GetGradient() {
            return -A_ / B_;
        }

        /**
            Get the line y-intercept
        */
        Real_t GetYIntercept() {
            return -C_ / B_;
        }

    };

}
}

#endif