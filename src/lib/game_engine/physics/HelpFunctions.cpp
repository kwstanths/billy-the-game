#include "HelpFunctions.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

namespace game_engine {

    float GetRadians(float degrees) {
        return M_PI / 180.0f * degrees;
    }


}
