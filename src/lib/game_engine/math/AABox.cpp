#include "AABox.hpp"

#include <algorithm>

namespace game_engine {
namespace math {

    bool IntersectionAABoxRay2D(AABox<2>& box, Ray2D ray, Real_t& t) {

        Real_t inv[2];
        inv[0] = Real_t(1) / ray.Direction()[0];
        inv[1] = Real_t(1) / ray.Direction()[1];

        Real_t t1 = (box.min_[0] - ray.Origin()[0])*inv[0];
        Real_t t2 = (box.max_[0] - ray.Origin()[0])*inv[0];
        Real_t t3 = (box.min_[1] - ray.Origin()[1])*inv[1];
        Real_t t4 = (box.max_[1] - ray.Origin()[1])*inv[1];

        Real_t tmin = std::max(std::min(t1, t2), std::min(t3, t4));
        Real_t tmax = std::min(std::max(t1, t2), std::max(t3, t4));

        t = tmax;
        if (tmax < 0) return false;
        if (tmin > tmax) return false;

        t = tmin;
        return true;
    }

}
}