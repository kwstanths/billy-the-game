#ifdef _WIN32
#include <Windows.h>
#endif

#include "debug_tools/CodeReminder.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/FrameRateRegulator.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;

void displayFPS(double frame_time_ms) {

    /* That's so nasty */
    static double start;
    static unsigned int frames;

    start += frame_time_ms;
    frames++;
    if (start >= 1000.0f) {
       
        std::cout << "FPS: " << frames << std::endl;

        start = 0.0;
        frames = 0;
    }

}

int main(int argc, char ** argv) {

    ge::GameEngine engine;
    ge::FrameRateRegulator frame_regulator;
    
    engine.Init(3, 3, 640, 480, "Billy");
    frame_regulator.Init(30);

    do {
        frame_regulator.FrameStart();

        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        }

        engine.Step();

        frame_regulator.FrameEnd();
        displayFPS(frame_regulator.GetDelta() * 1000.0);
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}