#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"
#include "game_engine/FrameRateRegulator.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "World.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


int main(int argc, char ** argv) {

    CodeReminder("Frame reate regulator inside the engine loop");
    CodeReminder("Implement the destruction of an object");
    CodeReminder("WorldObject, collision in SetPosition");
    CodeReminder("WorldSector, remove array add quad tree")

    ge::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/arial.ttf";
    context_params.shader_vertex_file_path_ = "shaders/VertexShader.glsl";
    context_params.shader_fragment_file_path_ = "shaders/FragmentShader.glsl";
    context_params.shader_vertex_text_file_path = "shaders/TextVertexShader.glsl";
    context_params.shader_fragment_text_file_path = "shaders/TextFragmentShader.glsl";
    ge::OpenGLCameraConfig_t camera_params;
    camera_params.pos_x_ = 0;
    camera_params.pos_y_ = 0;
    camera_params.pos_z_ = 8;
    camera_params.dir_x_ = 0;
    camera_params.dir_y_ = 1;
    camera_params.dir_z_ = 0;
    camera_params.up_x_ = 0;
    camera_params.up_y_ = 1;
    camera_params.up_z_ = 0;
    camera_params.orthographic_ = false;
    camera_params.zoom_factor_ = 75;
    ge::GameEngine engine(context_params, camera_params);
    if (engine.Init()) return false;
    
    World world;
    world.Init(&engine);

    engine.SetWorld(&world);

    ge::FrameRateRegulator frame_regulator;
    frame_regulator.Init(100, 5);
    do {
        frame_regulator.FrameStart();
        float delta_time = frame_regulator.GetDelta();
        
        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        }

        if (input.KEY_PAGE_UP) engine.CameraZoom2D(10 * delta_time);
        if (input.KEY_PAGE_DOWN) engine.CameraZoom2D(-10 * delta_time);

        engine.Step(delta_time);

        frame_regulator.FrameEnd();
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}