#include "HelpFunctions.hpp"

namespace game_engine {
namespace math {

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

}
}
