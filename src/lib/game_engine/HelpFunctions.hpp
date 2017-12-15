#ifndef __HelpFunctions_hpp__
#define __HelpFunctions_hpp__

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

}

#endif