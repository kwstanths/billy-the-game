#include "Vector3.hpp"

#include "HelpFunctions.hpp"

namespace game_engine {
namespace math {
    
    Vector3::Vector3(Vector3 & start, Vector3 & end) {
        Vector3 direction_vector = end - start;

        x_ = direction_vector.x_;
        y_ = direction_vector.y_;
        z_ = direction_vector.z_;
    }

    Vector3::Vector3(Vector3 const& other) {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
    }

    Vector3 Vector3::operator+(const Vector3 & other) {
        return Vector3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Vector3 Vector3::operator-(const Vector3 & other) {
        return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    Vector3 Vector3::operator*(Real_t n) {
        return Vector3(x_ * n, y_ * n, z_ * n);
    }

    bool Vector3::operator==(const Vector3 & other) {
        return Equal(x_, other.x_) && Equal(y_, other.y_) && Equal(z_, other.z_);
    }

    std::ostream & operator<<(std::ostream & stream, const Vector3 & v) {
        stream << "[" << v.x_ << ", " << v.y_ << ", " << v.z_ << "]";
        return stream;
    }

    Real_t Vector3::GetX() {
        return x_;
    }

    Real_t Vector3::GetY() {
        return y_;
    }

    Real_t Vector3::GetZ() {
        return z_;
    }

    Real_t Vector3::Norm() {
        return sqrt(x_*x_ + y_ * y_ + z_ * z_);
    }

    void Vector3::Normalize() {
        Real_t norm = Norm();
        x_ = x_ / norm;
        y_ = y_ / norm;
        z_ = z_ / norm;
    }

    Real_t Vector3::DotProduct(Vector3 const& a, Vector3 const& b) {
        return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
    }

    Vector3 Vector3::CrossProduct(Vector3 const & a, Vector3 const & b) {
        return Vector3(a.y_*b.z_ - a.z_*b.y_,
            a.z_*b.x_ - a.x_*b.z_,
            a.x_*b.y_ - a.y_*b.x_
        );
    }

    Vector3 Vector3::ProjectionOn(Vector3 const& other) {
        Vector3 other_unit = other;
        other_unit.Normalize();
        return DotProduct(*this, other_unit) * other_unit;
    }

    Vector3 operator*(Real_t n, Vector3 & other) {
        return Vector3(other.GetX() * n, other.GetY() * n, other.GetZ() *n);
    }

}
}