#ifndef __Vector_hpp__
#define __Vector_hpp__

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "Real.hpp"
#include "HelpFunctions.hpp"
#include "Vec3.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace math {

    template<int K>
    class Vector {
    public:
        Vector() {
            coordinates_ = std::vector<Real_t>(K, 0);
        }

        Vector(Real_t a) {
            coordinates_ = std::vector<Real_t>(K, a);
        }

        Vector(std::vector<Real_t> coordinates) {
            coordinates_ = coordinates;
        }

        Real_t Norm() {
            Real_t a = 0;
            for (size_t i = 0; i < K; i++) {
                a += std::pow<Real_t>(coordinates_[i], Real_t(2));
            }
            return sqrt(a);
        }

        void Normalise() {
            Real_t norm = Norm();
            for (size_t i = 0; i < K; i++) {
                coordinates_[i] = coordinates_[i] / norm;
            }
        }

        static Real_t Distance(Vector<K>& a, Vector<K>& b) {
            Real_t sum = 0;
            for (size_t i = 0; i < K; i++) {
                sum += std::pow<Real_t>(a[i] - b[i], 2);
            }

        }

        Real_t DotProduct(Vector<K>& a) {
            Real_t sum = 0;
            for (size_t i = 0; i < K; i++) {
                sum += coordinates_[i] * a.coordinates_[i];
            }
            return sum;
        }

        Vector& operator+=(const Vector& rhs) {
            for (size_t i = 0; i < K; i++)
                coordinates_[i] += rhs.coordinates_[i];
            return *this;
        }
        friend Vector operator+(Vector lhs, const Vector& rhs) {
            lhs += rhs;
            return lhs;
        }

        Vector& operator-=(const Vector& rhs) {
            for (size_t i = 0; i < K; i++)
                coordinates_[i] -= rhs.coordinates_[i];
            return *this;
        }
        friend Vector operator-(Vector lhs, const Vector& rhs) {
            lhs -= rhs;
            return lhs;
        }

        friend Vector operator*(Real_t v, const Vector& rhs) {
            Vector3D temp = rhs;
            for (size_t i = 0; i < K; i++)
                temp.coordinates_[i] = temp.coordinates_[i] * v;
            return temp;
        }
        friend Vector operator*(const Vector& lhs, Real_t v) {
            Vector3D temp = lhs;
            for (size_t i = 0; i < K; i++)
                temp.coordinates_[i] = temp.coordinates_[i] * v;
            return temp;
        }

        friend Vector operator/(const Vector& lhs, Real_t v) {
            Vector3D temp = lhs;
            for (size_t i = 0; i < K; i++)
                temp.coordinates_[i] = temp.coordinates_[i] / v;
            return temp;
        }

        bool operator==(const Vector& rhs) {
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

        friend std::ostream& operator<<(std::ostream& os, Vector const& h) {
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

    class Vector2D : public Vector<2> {
    public:
        Vector2D() : Vector() { };

        Vector2D(Real_t a) : Vector(a) { };

        Vector2D(std::vector<Real_t> coordinates) : Vector(coordinates) { }

        Vector2D(glm::vec2& v) {
            coordinates_[0] = v.x;
            coordinates_[1] = v.y;
        }

        Vector2D(Vector<2>& v) : Vector() {
            coordinates_[0] = v[0];
            coordinates_[1] = v[1];
        }

        Vector2D& operator = (Vector<2>& v) {
            return Vector2D({v[0], v[1]});
        }

        Real_t& x() {
            return coordinates_[0];
        }

        Real_t& y() {
            return coordinates_[1];
        }

        glm::vec2 ToGlm() {
            return glm::vec2(coordinates_[0], coordinates_[1]);
        }
    };

    class Vector3D : public Vector<3> {
    public:
        Vector3D() : Vector() { };

        Vector3D(Real_t a) : Vector(a) { };

        Vector3D(std::vector<Real_t> coordinates) : Vector(coordinates) { }

        Vector3D(glm::vec3& v) : Vector() {
            coordinates_[0] = v.x;
            coordinates_[1] = v.y;
            coordinates_[2] = v.z;
        }

        Vector3D(Vector<3>& v) : Vector() {
            coordinates_[0] = v[0];
            coordinates_[1] = v[1];
            coordinates_[2] = v[2];
        }

        Vector3D(Vec3& v) : Vector() {
            coordinates_[0] = v.x_;
            coordinates_[1] = v.y_;
            coordinates_[2] = v.z_;
        }

        static Vector3D CrossProduct(Vector3D& a, Vector3D& b) {
            return Vector3D(
                { a.y()*b.z() - a.z()*b.y(),
                a.z()*b.x() - a.x()*b.z(),
                a.x()*b.y() - a.y()*b.x()
                });
        }

        Vector3D& operator = (Vector<3>& v) {
            return Vector3D({ v[0], v[1], v[2] });
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

        glm::vec3 ToGlm() {
            return glm::vec3(coordinates_[0], coordinates_[1], coordinates_[2]);
        }
    };

}
}

#endif
