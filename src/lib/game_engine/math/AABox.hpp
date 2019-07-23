#ifndef __AABox_hpp__
#define __AABox_hpp__

#include "Real.hpp"
#include "Point.hpp"

namespace game_engine {
namespace math {

    
    template<int K>
    class AABox {
    public:

        AABox() {
            min_ = Point<K>(-1);
            max_ = Point<K>(1);
        }
        AABox(Point<K> min, Point<K> max) {
            min_ = min;
            max_ = max;
        }

        static bool Overlaps(AABox<K>& a, AABox<K>& b) {
            return overlaping(a, b);
        }

        bool Overlaps(AABox<K>& other) {
            return overlaping(*this, other);
        }

        bool Inside(Point<K> p) {
            bool inside = true;
            for (size_t i = 0; i < K; i++)
                inside = inside && (min_[i] <= p[i])  && (p[i] <= max_[i]);
            return inside;
        }

        float GetDiagonalDistance() {
            return min_.Distance(max_);
        }

        Point<K> min_;
        Point<K> max_;

    private:

        static bool overlaping(AABox<1>& a, AABox<1>& b) {
            if (a.max_[0] >= b.min_[0] && b.max_[0] >= a.min_[0]) return true;
            return false;
        }

        static bool overlaping(AABox<2>& a, AABox<2>& b) {
            if (overlaping(AABox<1>(Point<1>(a.min_[0]), Point<1>(a.max_[0])), AABox<1>(Point<1>(b.min_[0]), Point<1>(b.max_[0])))
                && overlaping(AABox<1>(Point<1>(a.min_[1]), Point<1>(a.max_[1])), AABox<1>(Point<1>(b.min_[1]), Point<1>(b.max_[1])))) return true;
            return false;
        }

        static bool overlaping(AABox<3>& a, AABox<3>& b) {
            if (overlaping(AABox<1>(Point<1>(a.min_[0]), Point<1>(a.max_[0])), AABox<1>(Point<1>(b.min_[0]), Point<1>(b.max_[0])))
                && overlaping(AABox<1>(Point<1>(a.min_[1]), Point<1>(a.max_[1])), AABox<1>(Point<1>(b.min_[1]), Point<1>(b.max_[1])))
                && overlaping(AABox<1>(Point<1>(a.min_[2]), Point<1>(a.max_[2])), AABox<1>(Point<1>(b.min_[2]), Point<1>(b.max_[2])))) return true;
            return false;
        }

    };

}
}



#endif