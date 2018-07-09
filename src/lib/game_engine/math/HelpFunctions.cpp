#include "HelpFunctions.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>

namespace game_engine {

    float M_PI_FLOAT = static_cast<float>(M_PI);

    float GetRadians(float degrees) {
        return M_PI_FLOAT / 180.0f * degrees;
    }

    float GetDegrees(float radians) {
        return radians * 180.0f / M_PI_FLOAT;
    }

    float InterpolationLinear(float start, float end, float percentage) {
        percentage = clamp(percentage, 0.0f, 1.0f);
        return start * (1 - percentage) + end * percentage;
    }

    float InterpolationCosine(float start, float end, float percentage) {
        float ft = percentage * M_PI_FLOAT;
        float f = (1.0f - cos(ft)) * 0.5f;
        return start * (1.0f - f) + end * f;

    }

}
