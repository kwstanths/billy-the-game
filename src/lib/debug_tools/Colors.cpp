#include "Colors.hpp"

namespace debug_tools{

    int GetWindowsColorCombination(Color fg_color, Color bg_color){
        return bg_color * 16 + fg_color;
    }

    int GetLinuxBackgroundColor(Color color){
        return color + 10;
    }
}

