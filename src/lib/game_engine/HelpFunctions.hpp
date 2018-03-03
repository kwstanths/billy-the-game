#ifndef __HelpFunctions_hpp__
#define __HelpFunctions_hpp__

#include <cstdlib>
#include <cmath>
#include <limits>

namespace game_engine {
    
    /**
        Check if two double numbers are equal
        @param a First number
        @param b Second number
        @return true=equal, false=not equal
    */
    template<typename T> bool Equal(T a, T b, T e = std::numeric_limits<T>::epsilon()) {
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
        Get radians from degrees
        @param degrees
        @return radians
    */
    float GetRadians(float degrees);
}

#endif
