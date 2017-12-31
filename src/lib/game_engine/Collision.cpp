#include "Collision.hpp"

#include <cmath>

namespace game_engine {
    
    bool CollisionCheck2DRectangles(float a_x, float a_y, float a_x_width, float a_y_height, 
        float b_x, float b_y, float b_x_width, float b_y_height)
    {
        if ((std::abs(a_x - b_x) * 2.0 < (a_x_width + b_x_width)) && (std::abs(a_y - b_y) * 2.0 < (a_y_height + b_y_height))) {
            return true;
        }

        return false;
    }
}
