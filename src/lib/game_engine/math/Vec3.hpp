#ifndef __Vec3_hpp__
#define __Vec3_hpp__

#include <glm/glm.hpp>

#include "Types.hpp"

namespace game_engine {
namespace math {

    class Vec3 {
    public:
        game_engine::Real_t x_, y_, z_;
        
        /** Create a zero vector */
        Vec3() : x_(0), y_(0), z_(0) {};

        /** Create a [n,n,n] vector */
        Vec3(Real_t n) : x_(n), y_(n), z_(n) {};

        /** Create a [x,y,z] vector */
        Vec3(Real_t x, Real_t y, Real_t z) : x_(x), y_(y), z_(z) {};

        /** Create a Vec3 from a glm::vec3 */
        Vec3(glm::vec3 v) : x_(v.x), y_(v.y), z_(v.z) {};

        /** Create a direction vector from start to end */
        Vec3(Vec3& start, Vec3& end);
        Vec3(Vec3 const& other);
        
        /** Set the coordinates of this vector */
        void Set(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z);

        Vec3 operator+(const Vec3& other);
        Vec3 operator-(const Vec3& other);
        Vec3 operator*(Real_t n);
        bool operator==(const Vec3& other);
        friend std::ostream& operator<< (std::ostream& stream, const Vec3& v);

        /** Calculate the norm of the vector */
        Real_t Norm();

        /** Normalize the vector */
        void Normalize();

        static Real_t DotProduct(Vec3 const& a, Vec3 const& b);
        Real_t DotProduct(Vec3 const& other);
        
        static Vec3 CrossProduct(Vec3 const& a, Vec3 const& b);
        Vec3 CrossProduct(Vec3 const& other);

        /** Calculate the projection of this vector on another vector */
        Vec3 ProjectionOn(Vec3 const& other);
    };

    Vec3 operator*(Real_t n, Vec3 & other);

}
}


#endif