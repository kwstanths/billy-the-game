#include "HelpFunctions.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>

namespace game_engine {

    float GetRadians(float degrees) {
        return M_PI / 180.0f * degrees;
    }

    float GetDegrees(float radians) {
        return radians * 180.0f / M_PI;
    }

    float InterpolationLinear(float start, float end, float percentage) {
        percentage = clamp(percentage, 0.0f, 1.0f);
        return start * (1 - percentage) + end * percentage;
    }

    float InterpolationCosine(float start, float end, float percentage) {
        double ft = percentage * M_PI;
        double f = (1.0 - cos(ft)) * 0.5;
        return start * (1.0f - f) + end * f;

    }

}
