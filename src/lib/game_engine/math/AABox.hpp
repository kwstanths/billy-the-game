#ifndef __AABox_hpp__
#define __AABox_hpp__

#include <vector>

#include "Real.hpp"
#include "Vector.hpp"
#include "Types.hpp"

namespace game_engine { namespace math {

    template<int K>
    class AABox {
    public:
    
        AABox() {
            min_ = Vector<K>(-1);
            max_ = Vector<K>(1);
        }
        AABox(Vector<K> min, Vector<K> max) {
            min_ = min;
            max_ = max;
        }
        AABox(Vector<K> center, std::vector<Real_t> length) {
            for (size_t i = 0; i < K; i++) {
                min_[i] = center[i] - length[i] / Real_t(2);
                max_[i] = center[i] + length[i] / Real_t(2);
            }
        }
    
        void Translate(Vector<K> translation) {
            for (size_t i = 0; i < K; i++) {
                min_[i] += translation[i];
                max_[i] += translation[i];
            }
        }
    
        static bool Overlaps(AABox<K>& a, AABox<K>& b) {
            return overlaping(a, b);
        }
    
        bool Overlaps(AABox<K>& other) {
            return overlaping(*this, other);
        }
    
        bool Inside(Vector<K> p) {
            bool inside = true;
            for (size_t i = 0; i < K; i++)
                inside = inside && (min_[i] <= p[i]) && (p[i] <= max_[i]);
            return inside;
        }
    
        float GetDiagonalDistance() {
            return min_.Distance(max_);
        }
    
        Vector<K> min_;
        Vector<K> max_;
    
    private:
    
        static bool overlaping(AABox<1>& a, AABox<1>& b) {
            if (a.max_[0] >= b.min_[0] && b.max_[0] >= a.min_[0]) return true;
            return false;
        }
    
        static bool overlaping(AABox<2>& a, AABox<2>& b) {
            if (overlaping(AABox<1>(Vector<1>(a.min_[0]), Vector<1>(a.max_[0])), AABox<1>(Vector<1>(b.min_[0]), Vector<1>(b.max_[0])))
                && overlaping(AABox<1>(Vector<1>(a.min_[1]), Vector<1>(a.max_[1])), AABox<1>(Vector<1>(b.min_[1]), Vector<1>(b.max_[1])))) return true;
            return false;
        }
    
        static bool overlaping(AABox<3>& a, AABox<3>& b) {
            if (overlaping(AABox<1>(Vector<1>(a.min_[0]), Vector<1>(a.max_[0])), AABox<1>(Vector<1>(b.min_[0]), Vector<1>(b.max_[0])))
                && overlaping(AABox<1>(Vector<1>(a.min_[1]), Vector<1>(a.max_[1])), AABox<1>(Vector<1>(b.min_[1]), Vector<1>(b.max_[1])))
                && overlaping(AABox<1>(Vector<1>(a.min_[2]), Vector<1>(a.max_[2])), AABox<1>(Vector<1>(b.min_[2]), Vector<1>(b.max_[2])))) return true;
            return false;
        }
    
    };
    
    bool IntersectionAABoxRay2D(AABox<2>& box, Ray2D ray, Real_t& t);

    /**
        Consider the box, and normal vector. The positive point returned from this function
        is the point on the box that is further along the normal. 
        See here http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
        @param normal The normal
        @return The closest point
    */
    Vector3D AAboxGetVertexPositive(AABox<3>& box, Vector3D& normal);

    /**
        Consider the box, and normal vector. The negative point returned from this function
        is the point on the box that is further away from the normal. 
        See here http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
        @param normal The normal
        @return The closest point
    */
    Vector3D AAboxGetVertexNegative(AABox<3>& box, Vector3D& normal);
}
}



#endif