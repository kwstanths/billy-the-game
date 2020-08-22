#ifndef __Types_hpp__
#define __Types_hpp__

#include <iostream>
#include <vector>

#include "Real.hpp"
#include "Vector.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
   

namespace game_engine {
namespace math {

    class Shape2D {
        virtual void Translate(Real_t x, Real_t y) = 0;
        virtual void Rotate(Real_t th) = 0;
    };

    /**
        A circle in a 2D pane
        c : center point
        r : radius
    */
    class Circle2D : public Shape2D {
    public:
        Vector2D c_;
        Real_t r_;


        Circle2D() {};
        Circle2D(Real_t x, Real_t y, Real_t r) {
            c_ = Vector2D(x, y);
            r_ = r;
        };

        /**
            Translate the circle
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(Real_t x, Real_t y) {
            c_.Translate({ x, y });
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
        Vector2D A_;
        Vector2D B_;
        Vector2D C_;
        Vector2D D_;

        Rectangle2D() {};

        Rectangle2D(Vector2D A, Vector2D B, Vector2D C, Vector2D D) : A_(A), B_(B), C_(C), D_(D) {};

        Rectangle2D(Real_t center_x, Real_t center_y, Real_t x_width, Real_t y_height) {
            Real_t xmar = x_width / 2.0f;
            Real_t ymar = y_height / 2.0f;

            A_ = Vector2D(center_x - xmar, center_y - ymar);
            B_ = Vector2D(center_x + xmar, center_y - ymar);
            C_ = Vector2D(center_x + xmar, center_y + ymar);
            D_ = Vector2D(center_x - xmar, center_y + ymar);
        }

        /*
            Translate the rectangle
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(Real_t x, Real_t y) {
            A_.Translate({x, y});
            B_.Translate({x, y});
            C_.Translate({x, y});
            D_.Translate({x, y});
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
            //Point2D center((B_.x() + D_.x()) / 2.0f, (B_.y() + D_.y()) / 2.0f);

            //A_.Rotate(th, center);
            //B_.Rotate(th, center);
            //C_.Rotate(th, center);
            //D_.Rotate(th, center);
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

    /**
        A ray in K dimensional space
    */
    template<int K>
    class Ray {
    public:
        Ray(Vector<K> origin, Vector<K> direction) : origin_(origin), direction_(direction) {
            direction_.Normalise();
        };

        Vector<K>& Origin() {
            return origin_;
        }

        Vector<K>& Direction() {
            return direction_;
        }

    private:
        Vector<K> origin_;
        Vector<K> direction_;
    };
    
    template<> class Ray<2> {
    public:
        Ray(Vector<2> origin, Real_t angle): origin_(origin) {
            direction_[0] = cos(GetRadians(angle));
            direction_[1] = sin(GetRadians(angle));
        }

        Vector<2>& Origin() {
            return origin_;
        }

        Vector<2>& Direction() {
            return direction_;
        }

    private:
        Vector<2> origin_;
        Vector<2> direction_;
    };


    typedef Ray<2> Ray2D;
    typedef Ray<3> Ray3D;

}
}

#endif