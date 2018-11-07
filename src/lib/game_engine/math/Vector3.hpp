#ifndef __Vector3_hpp__
#define __Vector3_hpp__

#include "Types.hpp"

namespace game_engine {
namespace math {

    class Vector3 {
    private:
        game_engine::Real_t x_;
        game_engine::Real_t y_;
        game_engine::Real_t z_;
    
    public:
        /**
            Create a zero vector
        */
        Vector3() : x_(0), y_(0), z_(0) {};

        /**
            Create a [n,n,n] vector
        */
        Vector3(Real_t n) : x_(n), y_(n), z_(n) {};

        /**
            Create a [x,y,z] vector
        */
        Vector3(Real_t x, Real_t y, Real_t z) : x_(x), y_(y), z_(z) {};

        /**
            Create a direction vector from start to end
        */
        Vector3(Vector3& start, Vector3& end);

        Vector3(Vector3 const& other);

        Vector3 operator+(const Vector3& other);

        Vector3 operator-(const Vector3& other);

        Vector3 operator*(Real_t n);

        bool operator==(const Vector3& other);

        friend std::ostream& operator<< (std::ostream& stream, const Vector3& v);

        /**
            Get x component
        */
        Real_t GetX();
        /**
            Get y component
        */
        Real_t GetY();
        /**
            Get z component
        */
        Real_t GetZ();

        /**
            Calculate the norm of the vector
        */
        Real_t Norm();

        /**
            Normalize the vector
        */
        void Normalize();

        /**
            Calculate the dot product between two vectors
        */
        static Real_t DotProduct(Vector3 const& a, Vector3 const& b);

        /**
            Calculate the cross product between two vectors
        */
        static Vector3 CrossProduct(Vector3 const& a, Vector3 const& b);

        /**
            Calculate the projection of this vector on another vector
        */
        Vector3 ProjectionOn(Vector3 const& other);

    };

    Vector3 operator*(Real_t n, Vector3 & other);

}
}


#endif