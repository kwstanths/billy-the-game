#include "HelpFunctions.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>

namespace game_engine {
namespace math {

    float M_PI_FLOAT = static_cast<float>(M_PI);

    Real_t GetRadians(Real_t degrees) {
        return static_cast<Real_t>(M_PI / 180.0 * degrees);
    }

    Real_t GetDegrees(Real_t radians) {
        return static_cast<Real_t>(radians * 180.0 / M_PI);
    }

    size_t PowerOfTwo(size_t v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }

    Real_t Lerp(Real_t start, Real_t end, Real_t percentage) {
        percentage = clamp(percentage, Real_t(0.0), Real_t(1.0));
        return start + percentage * (end - start);
    }

    Real_t LerpCosine(Real_t start, Real_t end, Real_t percentage) {
        Real_t ft = percentage * M_PI_FLOAT;
        Real_t f = (1.0f - cos(ft)) * 0.5f;
        return start * (1.0f - f) + end * f;
    }

}
}
