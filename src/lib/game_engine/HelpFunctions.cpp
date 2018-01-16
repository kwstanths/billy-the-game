#include "HelpFunctions.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

float game_engine::GetRadians(float degrees) {
    return M_PI / 180.0f * degrees;
}
