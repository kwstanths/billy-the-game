#ifndef __HelpFunctions_hpp__
#define __HelpFunctions_hpp__

#include <cstdlib>
#include <limits>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

namespace game_engine {
    
    /**
        Check if two double numbers are equal
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
        Get radians from degrees
        @param degrees
        @return radians
    */
    float GetRadians(float degrees);

    /**
        Get degrees from radians
        @param radians
        @return degrees
    */
    float GetDegrees(float radians);

    /**
    
    */
    float InterpolationLinear(float start, float end, float percentage);

    /**
    
    */
    float InterpolationCosine(float start, float end, float percentage);

}

#endif
