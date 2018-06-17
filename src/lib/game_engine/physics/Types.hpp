#ifndef __Types_hpp__
#define __Types_hpp__

#include "HelpFunctions.hpp"
#include "Matrices.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {
    
    /* Shows a direction in degrees: 0 = top, 90 = left, 180 = bottom, 270 = right */
    typedef float Direction;


    class Shape2D {
        virtual void Translate(float x, float y) = 0;
        virtual void Rotate(float th) = 0;
    };


    /**
        A point in a 2D pane 
        (x,y) : point coordinates
    */
    class Point2D {
    public:
        float x_;
        float y_;

        Point2D() {
            x_ = y_ = 0.0f;
        }
        Point2D(float x, float y) : x_(x), y_(y) {};

        bool operator==(Point2D a) const {
            if (Equal(a.x_, x_) && Equal(a.y_, y_)) return true;
            else return false;
        }

        /*
            Translate the point
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(float x, float y){
            x_ += x;
            y_ += y;
        }

        /**
            Rotate the point clockwise around another point
            @param th The rotation angle in radians
            @param axis The point to rotate around
        */
        void Rotate(float th, Point2D axis) {
            float c = cos(th);
            float s = sin(th);
            
            float x = x_ - axis.x_;
            float y = y_ - axis.y_;

            x_ = x * c + y * s;
            y_ = -x * s + y * c;

            x_ += axis.x_;
            y_ += axis.y_;
        }

    };

    /**
        A circle in a 2D pane 
        c : center point
        r : radius
    */
    class Circle2D : public Shape2D {
    public:
        Point2D c_;
        float r_;


        Circle2D() {};
        Circle2D(float x, float y, float r) {
            c_.x_ = x;
            c_.y_ = y;
            r_ = r;
        };

        /**
            Translate the circle
            @param x Horizontal movement
            @param y Vertical movement
        */
        void Translate(float x, float y){
            c_.Translate(x, y);
        }

        /**
            Scale the circle
            @param s The scaling amount
        */
        void Scale(float s) {
            r_ = r_ * s;
        }

        /**
            Rotate the circle clockwise around its center
            @param th The rotation angle in radians
        */
        void Rotate(float th){
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

        Rectangle2D(Point2D A, Point2D B, Point2D C, Point2D D): A_(A), B_(B), C_(C), D_(D) {};
        
        Rectangle2D(float center_x, float center_y, float x_width, float y_height) {
            float xmar = x_width / 2.0f;
            float ymar = y_height / 2.0f;

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
        void Translate(float x, float y){
            A_.Translate(x, y);
            B_.Translate(x, y);
            C_.Translate(x, y);
            D_.Translate(x, y);
        }

        /**
            Scale the rectangle
            @param s The scaling amount
        */
        void Scale(float s) {
            dt::Console(dt::CRITICAL, "Rectangle2D::Scale(): This function is not implemented");
        }

        /**
            Rotate the rectangle clockwise around its center
            @param th The rotation angle in radians
        */
        void Rotate(float th){
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
            C_ = a_y - gradient * a_x;
        };
        /**
            Create a line from a point and a slope
        */
        Line2D(float a_x, float a_y, float gradient) {
            A_ = gradient;
            B_ = -1.0f;
            C_ = a_y - gradient * a_x;
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
            return -C_ / B_;
        }

    };

}

#endif