#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"
#include "game_engine/ConsoleParser.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "World1.hpp"
#include "World2.hpp"
#include "World3.hpp"
#include "World4.hpp"
#include "Camera.hpp"

namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;
namespace dt = debug_tools;


int main(int argc, char ** argv) {

    /* Configuration parameters for the engine */
    gl::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1280;
    context_params.window_height_ = 920;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/Arial.ttf";
    ge::GameEngineConfig_t engine_params;
    engine_params.context_params_ = context_params;
    engine_params.frame_rate_ = 0;
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
    World1 world1;
    world1.Init(&input, camera, &engine);
    World2 world2;
    world2.Init(&input, camera, &engine);
    World3 world3;
    world3.Init(&input, camera, &engine);
    World4 world4;
    world4.Init(&input, camera, &engine);

    /* Set the active world in the engine */
    engine.SetWorld(&world);
    do {
        float delta_time = engine.GetFrameDelta();
        
        ControlInput_t controls = input.GetControls();
        if (controls.QUIT_) {
            break;
        }

        game_engine::ConsoleCommand c = game_engine::ConsoleParser::GetInstance().GetLastCommand();
        if (c.type_ == COMMAND_CA_WORLD) {
            if (game_engine::math::Equal(c.arg_1_, 0.0f))
                engine.SetWorld(&world);
            else if (game_engine::math::Equal(c.arg_1_, 1.0f))
                engine.SetWorld(&world1);
            else if (game_engine::math::Equal(c.arg_1_, 2.0f))
                engine.SetWorld(&world2);
            else if (game_engine::math::Equal(c.arg_1_, 3.0f))
                engine.SetWorld(&world3);
            else if (game_engine::math::Equal(c.arg_1_, 4.0f))
                engine.SetWorld(&world4);

        }
        

        if (controls.ZOOM_IN_) camera->Zoom(-10 * delta_time);
        if (controls.ZOOM_OUT_) camera->Zoom(10 * delta_time);

        float move_offset = 10 * static_cast<float>(delta_time);
        camera->KeyboardMoveFlightMode(controls.MOVE_UP_ * move_offset - controls.MOVE_DOWN_ * move_offset, -controls.MOVE_LEFT_ * move_offset + controls.MOVE_RIGHT_ * move_offset);

        engine.Step(delta_time);

    } while (1);

    engine.Destroy();
}