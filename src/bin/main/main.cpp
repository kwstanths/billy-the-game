#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Camera.hpp"

namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;
namespace dt = debug_tools;


int main(int argc, char ** argv) {

    CodeReminder("World sector, physics engine step");
    CodeReminder("Interact neighbour - ray casting");
    CodeReminder("Line drawing");
    CodeReminder("Fix the build with double");
    CodeReminder("Add the sun to something so that step and draw are always executed");
    CodeReminder("Add the point lights so that the ones closest are always drawn");
    CodeReminder("Draw with different shader");
    
    /* Configuration parameters for the engine */
    gl::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/KateCelebration.ttf";
    ge::GameEngineConfig_t engine_params;
    engine_params.context_params_ = context_params;
    engine_params.frame_rate_ = 100;
    ge::GameEngine engine;
    if (engine.Init(engine_params)) return false;
    
    /* Create a camera */
    camera = new Camera(context_params.window_width_, context_params.window_height_, 0.05f);
    engine.SetCamera(camera);
    camera->SetMouceCallback(MouseCallback);

    /* Initialize the input class */
    Input input;
    input.Init(&engine);

    /* Create a world */
    World world;
    world.Init(&input, camera, &engine);

    /* Set the active world in the engine */
    engine.SetWorld(&world);
    do {
        float delta_time = engine.GetFrameDelta();
        
        ControlInput_t controls = input.GetControls();
        if (controls.QUIT_) {
            break;
        }

        if (controls.ZOOM_IN_) camera->Zoom(-10 * delta_time);
        if (controls.ZOOM_OUT_) camera->Zoom(10 * delta_time);

        engine.Step(delta_time);

    } while (1);

    engine.Destroy();
}