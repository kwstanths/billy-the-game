#include "Vec3.hpp"

#include "HelpFunctions.hpp"

namespace game_engine {
namespace math {
    
    Vec3::Vec3(Vec3 & start, Vec3 & end) {
        Vec3 direction_vector = end - start;

        x_ = direction_vector.x_;
        y_ = direction_vector.y_;
        z_ = direction_vector.z_;
    }

    Vec3::Vec3(Vec3 const& other) {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
    }

    void Vec3::Set(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z) {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    Vec3 Vec3::operator+(const Vec3 & other) {
        return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Vec3 Vec3::operator-(const Vec3 & other) {
        return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    Vec3 Vec3::operator*(Real_t n) {
        return Vec3(x_ * n, y_ * n, z_ * n);
    }

    bool Vec3::operator==(const Vec3 & other) {
        return Equal(x_, other.x_) && Equal(y_, other.y_) && Equal(z_, other.z_);
    }

    std::ostream & operator<<(std::ostream & stream, const Vec3 & v) {
        stream << "[" << v.x_ << ", " << v.y_ << ", " << v.z_ << "]";
        return stream;
    }

    Real_t Vec3::Norm() {
        return sqrt(x_*x_ + y_ * y_ + z_ * z_);
    }

    void Vec3::Normalize() {
        Real_t norm = Norm();
        x_ = x_ / norm;
        y_ = y_ / norm;
        z_ = z_ / norm;
    }

    Real_t Vec3::DotProduct(Vec3 const& a, Vec3 const& b) {
        return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
    }

    Real_t Vec3::DotProduct(Vec3 const & other) {
        return DotProduct(Vec3(x_, y_, z_), other);
    }

    Vec3 Vec3::CrossProduct(Vec3 const & other) {
        return CrossProduct(Vec3(x_, y_, z_), other);
    }

    Vec3 Vec3::CrossProduct(Vec3 const & a, Vec3 const & b) {
        return Vec3(a.y_*b.z_ - a.z_*b.y_,
            a.z_*b.x_ - a.x_*b.z_,
            a.x_*b.y_ - a.y_*b.x_
        );
    }

    Vec3 Vec3::ProjectionOn(Vec3 const& other) {
        Vec3 other_unit = other;
        other_unit.Normalize();
        return DotProduct(*this, other_unit) * other_unit;
    }

    Vec3 operator*(Real_t n, Vec3 & other) {
        return Vec3(other.x_ * n, other.y_ * n, other.z_ *n);
    }

}
}