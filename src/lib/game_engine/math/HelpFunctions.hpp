#ifndef __HelpFunctions_hpp__
#define __HelpFunctions_hpp__

#include <cstdlib>
#include <limits>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>

#include "Real.hpp"

namespace game_engine {
namespace math {
    
    static const float M_PI_FLOAT = static_cast<float>(M_PI);

    /**
        Check if two numbers are equal, within a margin
        @param a First number
        @param b Second number
        @return true=equal, false=not equal
    */
    template<typename T> bool Equal(const T& a, const T& b, T e = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) <= e;
    }

    /**
        Swap two values, uses copy constructor
        @param a Value a
        @param b Value b
    */
    template<typename T> void Swap(T & a, T & b) {
        T temp;
        temp = a;
        a = b;
        b = temp;
    }

    /**
        Get the sign of a number
        @param val input number
        @return 1=positive, 0=zero, -1=negative
    */
    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    /**
        Clamp function. Restrict the value between two other values
        @param value The actual value
        @param low If value smaller than low, then returns low
        @param high If value greater than high, then returns high
        @return else returns the actual value
    */
    template <typename T> T clamp(const T& value, const T& low, const T& high) {
        return value < low ? low : (value > high ? high : value);
    }

    /**
        Map a value from one range of numbers to another range [in_a, in_b] --> [out_a, out_b]
        @param in_a Source range start
        @param in_b Source range end
        @param in_t number inside [in_a, in_b]
        @param out_a Target range start
        @param out_b Target range end
        @return in_t mapped to range [out_a, out_b]
    */
    template<typename T, typename F> static F map_to_range(T in_a, T in_b, T in_t, F out_a, F out_b) {
        return out_a + ((out_b - out_a) / (in_b - in_a)) * (in_t - in_a);
    }

    /**
        Get radians from degrees
        @param degrees
        @return radians
    */
    Real_t GetRadians(Real_t degrees);

    /**
        Get degrees from radians
        @param radians
        @return degrees
    */
    Real_t GetDegrees(Real_t radians);

    /**
        Find the nearest number higher than v, that is a power of two
        @param v Input number
        @return The closest power of two number higher than v
    */
    size_t PowerOfTwo(size_t v);

    /**

    */
    template<typename T> T Lerp(T start, T end, Real_t percentage) {
        percentage = clamp(percentage, Real_t(0.0), Real_t(1.0));
        return start + percentage * (end - start);
    }

    /**

    */
    template<typename T> T LerpCosine(T start, T end, Real_t percentage) {
        Real_t ft = percentage * M_PI_FLOAT;
        Real_t f = (1.0f - cos(ft)) * 0.5f;
        return start * (1.0f - f) + end * f;
    }

}
}

#endif
