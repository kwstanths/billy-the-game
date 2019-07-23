#include "Types.hpp"

#include "HelpFunctions.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace math {

   Line2D::Line2D(Real_t a_x, Real_t a_y, Real_t b_x, Real_t b_y) {
        if (Equal(b_x, a_x)) {
            A_ = 1.0f;
            B_ = 0.0f;
            C_ = -a_x;
            return;
        }
        Real_t gradient = (b_y - a_y) / (b_x - a_x);
        A_ = gradient;
        B_ = -1.0f;
        C_ = a_y - gradient * a_x;
    };


}
}