#ifndef __Point_hpp__
#define __Point_hpp__

#include <iostream>
#include <vector>

#include "Real.hpp"
#include "HelpFunctions.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace math {

    template<int K>
    class Point {
    public:
        Point() {
            coordinates_ = std::vector<Real_t>(K, 0);
        }

        Point(Real_t a) {
            coordinates_ = std::vector<Real_t>(K, a);
        }

        Point(std::vector<Real_t> coordinates) {
            coordinates_ = coordinates;
        }

        Real_t Norm() {
            Real_t a = 0;
            for (size_t i = 0; i < K; i++) {
                a += std::pow<Real_t>(coordinates_[i], 2);
            }
            return sqrt(a);
        }

        void Normalise() {
            Real_t norm = Norm();
            for (size_t i = 0; i < K; i++) {
                coordinates_[i] = coordinates_[i] / norm;
            }
        }

        Real_t Distance(Point<K>& a, Point<K>& b) {
            Real_t sum = 0;
            for (size_t i = 0; i < K; i++) {
                sum += std::pow<Real_t>(a[i] - b[i], 2);
            }

        }

        Point& operator+=(const Point& rhs) {
            for (size_t i = 0; i < K; i++)
                coordinates_[i] += rhs.coordinates_[i];
            return *this;
        }

        friend Point operator+(Point lhs, const Point& rhs) {
            lhs += rhs;
            return lhs;
        }

        Point& operator-=(const Point& rhs) {
            for (size_t i = 0; i < K; i++)
                coordinates_[i] -= rhs.coordinates_[i];
            return *this;
        }

        friend Point operator-(Point lhs, const Point& rhs) {
            lhs -= rhs;
            return lhs;
        }

        bool operator==(const Point& rhs) {
            bool ret = true;
            for (size_t i = 0; i < K; i++) {
                ret = ret & Equal(coordinates_[i], rhs.coordinates_[i]);
            }
            return ret;
        }

        Real_t& operator[](size_t i) {
            return coordinates_[i];
        }

        void Translate(std::vector<Real_t> translation) {
            for (size_t i = 0; i < K; i++) {
                coordinates_[i] += translation[i];
            }
        }

        Real_t& x() {
            return coordinates_[0];
        }

        Real_t& y() {
            return coordinates_[1];
        }

        Real_t& z() {
            return coordinates_[2];
        }

        friend std::ostream& operator<<(std::ostream& os, Point const& h) {
            os << "[";
            for (size_t i = 0; i < K - 1; i++) {
                os << h.coordinates_[i] << ", ";
            }
            os << h.coordinates_[0] << "]";
            return os;
        }

    protected:
        std::vector<Real_t> coordinates_;

    };

    typedef Point<2> Point2D;
    typedef Point<3> Point3D;

}
}

#endif
